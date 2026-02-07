#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include "OpenglClass.hpp"
#include <fstream>

class ModelClass {
private:
  struct VertexType {
    float x, y, z;
    float r, g, b;
  };

public:
  ModelClass();
  ModelClass(const ModelClass &);
  ~ModelClass();

  bool Initialize(OpenGLClass *);
  void Shutdown();
  void Render();

private:
  bool InitializeBuffers();
  void ShutdownBuffers();
  void RenderBuffers();

private:
  OpenGLClass *m_OpenGLPtr;
  int m_vertexCount, m_indexCount;
  unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;
};
#endif
