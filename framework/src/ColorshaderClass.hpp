#ifndef _COLORSHADERCLASS_H_
#define _COLORSHADERCLASS_H_

#include <cstdint>
#include <iostream>
using namespace std;

#include "OpenglClass.hpp"

class ColorShaderClass {
public:
  ColorShaderClass();
  ColorShaderClass(const ColorShaderClass &);
  ~ColorShaderClass();

  bool Initialize(OpenGLClass *);
  void Shutdown();

  bool SetShaderParameters(float *, float *, float *);

private:
  bool InitializeShader(char *, char *);
  void ShutdownShader();
  char *LoadShaderSourceFile(char *);
  void OutputShaderErrorMessage(unsigned int, char *);
  void OutputLinkerErrorMessage(unsigned int);

  OpenGLClass *m_OpenGLPtr;
  unsigned int m_vertexShader;
  unsigned int m_fragmentShader;
  unsigned int m_shaderProgram;
};

#endif
