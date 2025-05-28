#include "Compiletype.hpp"

#include "utilities/ESPadapter.hpp"

#ifdef DEPLOY
#include "Manager.hpp"
#include "utilities/Version.h"
#include "serial/SerialPort.hpp"

Manager manager;

uint32_t versionESP = 0;

void setup()
{
    manager.initialize();

    versionESP = (completeVersion[0] - 48) * 10000000L +
                 (completeVersion[1] - 48) * 1000000L +
                 (completeVersion[2] - 48) * 100000L +
                 (completeVersion[3] - 48) * 10000L +
                 (completeVersion[4] - 48) * 1000L +
                 (completeVersion[5] - 48) * 100L +
                 (completeVersion[6] - 48) * 10L +
                 (completeVersion[7] - 48) * 1L;
}

void loop()
{   
    manager.run();
    manager.sendInitializationData(versionESP);
}

#endif