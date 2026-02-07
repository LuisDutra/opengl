#include "ModelClass.hpp"
#include <GL/glext.h>

ModelClass::ModelClass() { m_OpenGLPtr = 0; }

ModelClass::ModelClass(const ModelClass &other) {}

ModelClass::~ModelClass() {}

bool ModelClass::Initialize(OpenGLClass *OpenGl) {
  bool result;

  m_OpenGLPtr = OpenGl;
  result = InitializeBuffers();

  if (!result) {
    return false;
  }

  return true;
}

void ModelClass::Shutdown() {
  ShutdownBuffers();

  m_OpenGLPtr = 0;

  return;
}

void ModelClass::Render() {
  RenderBuffers();

  return;
}

bool ModelClass::InitializeBuffers() {
  VertexType *vertices;
  unsigned int *indices;

  m_vertexCount = 3;

  m_indexCount = 3;

  vertices = new VertexType[m_vertexCount];

  indices = new unsigned int[m_indexCount];

  // Load the vertex array with data.

  // Bottom left.
  vertices[0].x = -1.0f; // Position.
  vertices[0].y = -1.0f;
  vertices[0].z = 0.0f;

  vertices[0].r = 0.0f; // Color.
  vertices[0].g = 1.0f;
  vertices[0].b = 0.0f;

  // Top middle.
  vertices[1].x = 0.0f; // Position.
  vertices[1].y = 1.0f;
  vertices[1].z = 0.0f;

  vertices[1].r = 0.0f; // Color.
  vertices[1].g = 1.0f;
  vertices[1].b = 0.0f;

  // Bottom right.
  vertices[2].x = 1.0f; // Position.
  vertices[2].y = -1.0f;
  vertices[2].z = 0.0f;

  vertices[2].r = 0.0f; // Color.
  vertices[2].g = 1.0f;
  vertices[2].b = 0.0f;

  // Load the index array with data.
  indices[0] = 0; // Bottom left.
  indices[1] = 1; // Top middle.
  indices[2] = 2; // Bottom right.

  m_OpenGLPtr->glGenVertexArrays(1, &m_vertexArrayId);

  m_OpenGLPtr->glBindVertexArray(m_vertexArrayId);

  m_OpenGLPtr->glGenBuffers(1, &m_vertexBufferId);

  m_OpenGLPtr->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);

  m_OpenGLPtr->glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(VertexType),
                            vertices, GL_STATIC_DRAW);

  m_OpenGLPtr->glEnableVertexAttribArray(0);
  m_OpenGLPtr->glEnableVertexAttribArray(1);

  m_OpenGLPtr->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexType),
                                     0);
  m_OpenGLPtr->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexType),
                                     (unsigned char *)NULL +
                                         (3 * sizeof(float)));

  m_OpenGLPtr->glGenBuffers(1, &m_indexBufferId);

  m_OpenGLPtr->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
  m_OpenGLPtr->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                            m_indexCount * sizeof(unsigned int), indices,
                            GL_STATIC_DRAW);

  delete[] vertices;
  vertices = 0;

  delete[] indices;
  indices = 0;

  return true;
}

void ModelClass::ShutdownBuffers() {
  // Release the vertex array object.
  m_OpenGLPtr->glBindVertexArray(0);
  m_OpenGLPtr->glDeleteVertexArrays(1, &m_vertexArrayId);

  // Release the vertex buffer.
  m_OpenGLPtr->glBindBuffer(GL_ARRAY_BUFFER, 0);
  m_OpenGLPtr->glDeleteBuffers(1, &m_vertexBufferId);

  // Release the index buffer.
  m_OpenGLPtr->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  m_OpenGLPtr->glDeleteBuffers(1, &m_indexBufferId);

  return;
}

void ModelClass::RenderBuffers() {
  // Bind the vertex array object that stored all the information about the
  // vertex and index buffers.
  m_OpenGLPtr->glBindVertexArray(m_vertexArrayId);

  // Render the vertex buffer using the index buffer.
  glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

  return;
}
