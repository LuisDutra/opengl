#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

#include <X11/X.h>
#include <X11/Xlib.h>
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_NEAR = 0.3f;
const float SCREEN_DEPTH = 1000.0f;

#include "InputClass.hpp"
#include "OpenglClass.hpp"

class ApplicationClass {
public:
  ApplicationClass();
  ApplicationClass(const ApplicationClass &);
  ~ApplicationClass();

  bool Initialize(Display *, Window, int, int);
  void Shutdown();
  bool Frame(InputClass *);

private:
  bool Render();
  OpenGLClass *m_OpenGL;
};

#endif
