#include "SystemClass.hpp"

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

  if (result) {
    return false;
  }
}

void SystemClass::Shutdown() { 
  if(m_Application){
    m_Application->Shutdown();
    delete m_Application;
    m_Application = 0;
  }

  ShutdownWindow();

  if(m_Input){
    delete m_Input;
    m_Input = 0;
  }

  return;
}

void SystemClass::Frame() { 
  bool done, result;

  done = false;

  while(!done){
    ReadInput();

    result = m_Application->Frame(m_Input);
    if(!result){
      done = true;
    }
  }

  return;
 }
