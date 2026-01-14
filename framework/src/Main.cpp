#include "SystemClass.hpp"
#include <ostream>
#include <iostream>

int main() {
    SystemClass* System;
    bool result;

    System = new SystemClass;

    result = System->Initialize();

    if(!result){
        return -1;
    }else{
        std::cout << "Positivo" << std::endl;
    }

    System->Frame();

    System->Shutdown();
    delete System;
    System = 0;

    return 0;
}