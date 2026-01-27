#include "SystemClass.hpp"
#include "ApplicationClass.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <cstddef>
#include <set>

SystemClass::SystemClass() {
  m_Input = 0;
  m_Application = 0;
}

SystemClass::SystemClass(const SystemClass &other) {}

SystemClass::~SystemClass() {}

bool SystemClass::Initialize() {
  int screenWidth, screenHeight;
  bool result;

  m_Input = new InputClass;

  m_Input->Initialize();

  screenWidth = 0;
  screenHeight = 0;

  result = InitializeWindow(screenWidth, screenHeight);

  if (!result) {
    return false;
  }

  m_Application = new ApplicationClass;

  result = m_Application->Initialize(m_videoDisplay, m_hwnd, screenWidth,
                                     screenHeight);

  if (!result) {
    return false;
  }
  return true;
}

void SystemClass::Shutdown() {
  if (m_Application) {
    m_Application->Shutdown();
    delete m_Application;
    m_Application = 0;
  }

  ShutdownWindow();

  if (m_Input) {
    delete m_Input;
    m_Input = 0;
  }

  return;
}

void SystemClass::Frame() {
  bool done, result;

  done = false;

  while (!done) {
    ReadInput();

    result = m_Application->Frame(m_Input);
    if (!result) {
      done = true;
    }
  }

  return;
}

bool SystemClass::InitializeWindow(int &screenWidth, int &screenHeight) {
  Window rootWindow;
  XVisualInfo *visualInfo;
  GLint attributeList[15];
  Colormap colorMap;
  XSetWindowAttributes setWindowAttributes;
  Screen *defaultScreen;
  bool result;
  int majorVersion;
  Atom wmState, fullScreenState, motifHints;
  XEvent fullScreenEvent;
  long motifHintList[5];
  int status, posX, posY, defaultScreenWidth, defaultScreenHeight;

  // Abre canexao com o servidor do X11
  m_videoDisplay = XOpenDisplay(NULL);
  if (m_videoDisplay == NULL) {
    return false;
  }

  rootWindow = DefaultRootWindow(m_videoDisplay);

  attributeList[0] = GLX_RGBA;       // Suporte para 24 bit de cor e canal alpha
  attributeList[1] = GLX_DEPTH_SIZE; // Suporte a buffers de depth 24 bits
  attributeList[2] = 24;
  attributeList[3] = GLX_STENCIL_SIZE; // Support for 8 bit stencil buffer.
  attributeList[4] = 8;
  attributeList[5] = GLX_DOUBLEBUFFER; // Support for double buffering.
  attributeList[6] = GLX_RED_SIZE;     // 8 bits for each channel.
  attributeList[7] = 8;
  attributeList[8] = GLX_GREEN_SIZE;
  attributeList[9] = 8;
  attributeList[10] = GLX_BLUE_SIZE;
  attributeList[11] = 8;
  attributeList[12] = GLX_ALPHA_SIZE;
  attributeList[13] = 8;
  attributeList[14] = None; // Null terminate the attribute list.

  visualInfo = glXChooseVisual(m_videoDisplay, 0, attributeList);

  if (visualInfo == NULL) {
    return false;
  }
  colorMap = XCreateColormap(m_videoDisplay, rootWindow, visualInfo->visual,
                             AllocNone);

  setWindowAttributes.colormap = colorMap;
  setWindowAttributes.event_mask =
      KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask;

  if (FULL_SCREEN) {
    defaultScreen = XDefaultScreenOfDisplay(m_videoDisplay);
    screenWidth = XWidthOfScreen(defaultScreen);
    screenHeight = XHeightOfScreen(defaultScreen);
  } else {
    screenWidth = 1024;
    screenHeight = 768;
  }

  m_hwnd =
      XCreateWindow(m_videoDisplay, rootWindow, 0, 0, screenWidth, screenHeight,
                    0, visualInfo->depth, InputOutput, visualInfo->visual,
                    CWColormap | CWEventMask, &setWindowAttributes);

  XMapWindow(m_videoDisplay, m_hwnd);

  XStoreName(m_videoDisplay, m_hwnd, "Engine");

  if (FULL_SCREEN) {
    // Setup the full screen states.
    wmState = XInternAtom(m_videoDisplay, "_NET_WM_STATE", False);
    fullScreenState =
        XInternAtom(m_videoDisplay, "_NET_WM_STATE_FULLSCREEN", False);

    // Setup the X11 event message that we need to send to make the screen go
    // full screen mode.
    memset(&fullScreenEvent, 0, sizeof(fullScreenEvent));

    fullScreenEvent.type = ClientMessage;
    fullScreenEvent.xclient.window = m_hwnd;
    fullScreenEvent.xclient.message_type = wmState;
    fullScreenEvent.xclient.format = 32;
    fullScreenEvent.xclient.data.l[0] = 1;
    fullScreenEvent.xclient.data.l[1] = fullScreenState;
    fullScreenEvent.xclient.data.l[2] = 0;
    fullScreenEvent.xclient.data.l[3] = 0;
    fullScreenEvent.xclient.data.l[4] = 0;

    // Send the full screen event message to the X11 server.
    status = XSendEvent(
        m_videoDisplay, DefaultRootWindow(m_videoDisplay), False,
        SubstructureRedirectMask | SubstructureNotifyMask, &fullScreenEvent);
    if (status != 1) {
      return false;
    }

    // Setup the motif hints to remove the border in full screen mode.
    motifHints = XInternAtom(m_videoDisplay, "_MOTIF_WM_HINTS", False);

    motifHintList[0] = 2; // Remove border.
    motifHintList[1] = 0;
    motifHintList[2] = 0;
    motifHintList[3] = 0;
    motifHintList[4] = 0;

    // Change the window property and remove the border.
    status =
        XChangeProperty(m_videoDisplay, m_hwnd, motifHints, motifHints, 32,
                        PropModeReplace, (unsigned char *)&motifHintList, 5);
    if (status != 1) {
      return false;
    }

    // Flush the display to apply all the full screen settings.
    status = XFlush(m_videoDisplay);
    if (status != 1) {
      return false;
    }

    // Now sleep for one second for the flush to occur before making a gl
    // context, or if too early the screen sets squished in full screen
    // sometimes.
    sleep(1);
  }
  // Create an OpenGL rendering context.
  m_renderingContext =
      glXCreateContext(m_videoDisplay, visualInfo, NULL, GL_TRUE);
  if (m_renderingContext == NULL) {
    return false;
  }

  // Attach the OpenGL rendering context to the newly created window.
  result = glXMakeCurrent(m_videoDisplay, m_hwnd, m_renderingContext);
  if (!result) {
    return false;
  }

  // Check that OpenGL 4.0 is supported at a minimum.
  glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
  if (majorVersion < 4) {
    return false;
  }

  // Confirm that we have a direct rendering context.
  result = glXIsDirect(m_videoDisplay, m_renderingContext);
  if (!result) {
    return false;
  }

  // If windowed then move the window to the middle of the screen.
  if (!FULL_SCREEN) {
    defaultScreen = XDefaultScreenOfDisplay(m_videoDisplay);
    defaultScreenWidth = XWidthOfScreen(defaultScreen);
    defaultScreenHeight = XHeightOfScreen(defaultScreen);

    posX = (defaultScreenWidth - screenWidth) / 2;
    posY = (defaultScreenHeight - screenHeight) / 2;

    status = XMoveWindow(m_videoDisplay, m_hwnd, posX, posY);
    if (status != 1) {
      return false;
    }
  }

  return true;
}

void SystemClass::ShutdownWindow() {
  // Shutdown and close the window.
  glXMakeCurrent(m_videoDisplay, None, NULL);
  glXDestroyContext(m_videoDisplay, m_renderingContext);
  XUnmapWindow(m_videoDisplay, m_hwnd);
  XDestroyWindow(m_videoDisplay, m_hwnd);
  XCloseDisplay(m_videoDisplay);

  return;
}

void SystemClass::ReadInput() {
  XEvent event;
  long eventMask;
  bool foundEvent;
  char keyBuffer[32];
  KeySym keySymbol;

  eventMask = KeyPressMask | KeyReleaseMask;

  foundEvent = XCheckWindowEvent(m_videoDisplay, m_hwnd, eventMask, &event);

  if (foundEvent) {
    if (event.type == KeyPress) {
      XLookupString(&event.xkey, keyBuffer, sizeof(keyBuffer), &keySymbol,
                    NULL);
      m_Input->KeyDown((int)keySymbol);
    }
    // Key release.
    if (event.type == KeyRelease) {
      XLookupString(&event.xkey, keyBuffer, sizeof(keyBuffer), &keySymbol,
                    NULL);
      m_Input->KeyUp((int)keySymbol);
    }
  }
  return;
}
