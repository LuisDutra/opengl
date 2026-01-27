#include "ApplicationClass.hpp"
#include "OpenglClass.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>

ApplicationClass::ApplicationClass() { m_OpenGL = 0; }

ApplicationClass::ApplicationClass(const ApplicationClass &other) {}

ApplicationClass::~ApplicationClass() {}

bool ApplicationClass::Initialize(Display *display, Window win, int screenWidth,
                                  int screenHeight) {
  bool result;

  m_OpenGL = new OpenGLClass;

  result = m_OpenGL->Initialize(display, win, screenWidth, screenHeight,
                                SCREEN_NEAR, SCREEN_DEPTH, VSYNC_ENABLED);

  if (!result) {
    return false;
  }

  return true;
}

void ApplicationClass::Shutdown() {
  if (m_OpenGL) {
    m_OpenGL->Shutdown();
    delete m_OpenGL;
    m_OpenGL = 0;
  }

  return;
}

bool ApplicationClass::Frame(InputClass *Input) {
  bool result;

  if (Input->IsEscapePressed() == true) {
    return false;
  }

  result = Render();

  if (!result) {
    return false;
  }

  return true;
}

bool ApplicationClass::Render() {
  // Clear the buffers to begin the scene.
  m_OpenGL->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

  // Present the rendered scene to the screen.
  m_OpenGL->EndScene();

  return true;
}
