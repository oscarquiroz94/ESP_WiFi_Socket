#include "Compiletype.hpp"

#include "adapters/ESPadapter.hpp"

#ifdef DEPLOY
#include "Manager.hpp"
#include "serial/SerialPort.hpp"

Manager manager;

void setup()
{
    manager.initialize();
}

void loop()
{   
    manager.run();
}

#endif