#include "ColorshaderClass.hpp"
#include <GL/glext.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

ColorShaderClass::ColorShaderClass() { m_OpenGLPtr = 0; }

ColorShaderClass::ColorShaderClass(const ColorShaderClass &other) {}

ColorShaderClass::~ColorShaderClass() {}

bool ColorShaderClass::Initialize(OpenGLClass *OpenGL) {
  char vsFilename[128];
  char psFilename[128];
  bool result;

  m_OpenGLPtr = OpenGL;

  strcpy(vsFilename, "../src/color.vs");
  strcpy(psFilename, "../src/color.ps");

  result = InitializeShader(vsFilename, psFilename);

  if (!result) {
    return false;
  }

  return true;
}

void ColorShaderClass::Shutdown() {
  ShutdownShader();

  m_OpenGLPtr = 0;

  return;
}

bool ColorShaderClass::InitializeShader(char *vsFilename, char *psFilename) {
  const char *vertexShaderBuffer;
  const char *pixelShaderBuffer;

  int status;

  vertexShaderBuffer = LoadShaderSourceFile(vsFilename);

  if (!vertexShaderBuffer) {
    return false;
  }

  pixelShaderBuffer = LoadShaderSourceFile(psFilename);

  if (!pixelShaderBuffer) {
    return false;
  }

  m_vertexShader = m_OpenGLPtr->glCreateShader(GL_VERTEX_SHADER);
  m_fragmentShader = m_OpenGLPtr->glCreateShader(GL_FRAGMENT_SHADER);

  m_OpenGLPtr->glShaderSource(m_vertexShader, 1, &vertexShaderBuffer, NULL);
  m_OpenGLPtr->glShaderSource(m_fragmentShader, 1, &pixelShaderBuffer, NULL);

  delete[] vertexShaderBuffer;
  vertexShaderBuffer = 0;

  delete[] pixelShaderBuffer;
  pixelShaderBuffer = 0;

  m_OpenGLPtr->glCompileShader(m_vertexShader);
  m_OpenGLPtr->glCompileShader(m_fragmentShader);

  m_OpenGLPtr->glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &status);

  if (status != 1) {
    OutputShaderErrorMessage(m_vertexShader, vsFilename);
    return false;
  }

  m_OpenGLPtr->glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &status);

  if (status != 1) {
    OutputShaderErrorMessage(m_fragmentShader, psFilename);
    return false;
  }

  m_shaderProgram = m_OpenGLPtr->glCreateProgram();

  m_OpenGLPtr->glAttachShader(m_shaderProgram, m_vertexShader);
  m_OpenGLPtr->glAttachShader(m_shaderProgram, m_fragmentShader);

  m_OpenGLPtr->glBindAttribLocation(m_shaderProgram, 0, "inputPosition");
  m_OpenGLPtr->glBindAttribLocation(m_shaderProgram, 1, "inputColor");

  m_OpenGLPtr->glLinkProgram(m_shaderProgram);

  m_OpenGLPtr->glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);

  if (status != 1) {
    OutputLinkerErrorMessage(m_shaderProgram);
    return false;
  }

  return true;
}

void ColorShaderClass::ShutdownShader() {
  m_OpenGLPtr->glDetachShader(m_shaderProgram, m_vertexShader);
  m_OpenGLPtr->glDetachShader(m_shaderProgram, m_fragmentShader);

  m_OpenGLPtr->glDeleteShader(m_vertexShader);
  m_OpenGLPtr->glDeleteShader(m_fragmentShader);

  m_OpenGLPtr->glDeleteProgram(m_shaderProgram);

  return;
}

char *ColorShaderClass::LoadShaderSourceFile(char *filename) {
  FILE *filePtr;
  char *buffer;
  long fileSize, count;
  int error;

  filePtr = fopen(filename, "r");

  if (filePtr == NULL) {
    return 0;
  }

  fseek(filePtr, 0, SEEK_END);
  fileSize = ftell(filePtr);

  buffer = new char[fileSize + 1];

  fseek(filePtr, 0, SEEK_SET);

  count = fread(buffer, 1, fileSize, filePtr);

  if (count != fileSize) {
    return 0;
  }

  error = fclose(filePtr);

  if (error != 0) {
    return 0;
  }

  buffer[fileSize] = '\0';

  return buffer;
}

void ColorShaderClass::OutputShaderErrorMessage(unsigned int shaderId,
                                                char *shaderFilename) {
  long count;
  int logSize, error;
  char *infoLog;
  FILE *filePtr;

  m_OpenGLPtr->glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);

  logSize++;

  infoLog = new char[logSize];

  m_OpenGLPtr->glGetShaderInfoLog(shaderId, logSize, NULL, infoLog);

  filePtr = fopen("shader-error.txt", "w");

  if (filePtr == NULL) {
    cout << "Error opening shader error message output file" << endl;
    return;
  }

  error = fclose(filePtr);

  if (error != 0) {
    cout << "Error closing shader error message output file" << endl;
    return;
  }

  cout << "Error compiling shader.  Check shader-error.txt for error message.  "
          "Shader filename: "
       << shaderFilename << endl;

  return;
}

void ColorShaderClass::OutputLinkerErrorMessage(unsigned int programId) {
  long count;
  FILE *filePtr;
  int logSize, error;
  char *infoLog;

  // Get the size of the string containing the information log for the failed
  // shader compilation message.
  m_OpenGLPtr->glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logSize);

  // Increment the size by one to handle also the null terminator.
  logSize++;

  // Create a char buffer to hold the info log.
  infoLog = new char[logSize];

  // Now retrieve the info log.
  m_OpenGLPtr->glGetProgramInfoLog(programId, logSize, NULL, infoLog);

  // Open a file to write the error message to.
  filePtr = fopen("linker-error.txt", "w");
  if (filePtr == NULL) {
    cout << "Error opening linker error message output file." << endl;
    return;
  }

  // Write out the error message.
  count = fwrite(infoLog, sizeof(char), logSize, filePtr);
  if (count != logSize) {
    cout << "Error writing linker error message output file." << endl;
    return;
  }

  // Close the file.
  error = fclose(filePtr);
  if (error != 0) {
    cout << "Error closing linker error message output file." << endl;
    return;
  }

  // Pop a message up on the screen to notify the user to check the text file
  // for linker errors.
  cout << "Error linking shader program.  Check linker-error.txt for message."
       << endl;

  return;
}

bool ColorShaderClass::SetShaderParameters(float *worldMatrix,
                                           float *viewMatrix,
                                           float *projectionMatrix) {
  float tpWorldMatrix[16], tpViewMatrix[16], tpProjectionMatrix[16];
  int location;

  m_OpenGLPtr->MatrixTranspose(tpWorldMatrix, worldMatrix);
  m_OpenGLPtr->MatrixTranspose(tpViewMatrix, viewMatrix);
  m_OpenGLPtr->MatrixTranspose(tpProjectionMatrix, projectionMatrix);

  m_OpenGLPtr->glUseProgram(m_shaderProgram);

  location = m_OpenGLPtr->glGetUniformLocation(m_shaderProgram, "worldMatrix");

  if (location == -1) {
    cout << "Worl matrix is not set" << endl;
  }

  m_OpenGLPtr->glUniformMatrix4fv(location, 1, false, tpViewMatrix);

  location = m_OpenGLPtr->glGetUniformLocation(m_shaderProgram, "viewMatrix");

  if (location == -1) {
    cout << "View matrix is not set." << endl;
  }
  m_OpenGLPtr->glUniformMatrix4fv(location, 1, false, tpViewMatrix);

  // Set the projection matrix in the vertex shader.
  location =
      m_OpenGLPtr->glGetUniformLocation(m_shaderProgram, "projectionMatrix");
  if (location == -1) {
    cout << "Projection matrix is not set." << endl;
  }
  m_OpenGLPtr->glUniformMatrix4fv(location, 1, false, tpProjectionMatrix);

  return true;
}
