#include "SystemClass.hpp"
#include <iostream>
#include <ostream>

int main() {
  SystemClass *System;
  bool result;

  System = new SystemClass;

  result = System->Initialize();

  if (!result) {
    return -1;
  }

  System->Frame();

  System->Shutdown();
  delete System;
  System = 0;

  return 0;
}
