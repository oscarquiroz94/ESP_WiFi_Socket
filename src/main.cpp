#include "Compiletype.hpp"

#include "ESPadapter.hpp"

#ifdef DEPLOY
#include "Manager.hpp"
#include "Version.h"

Manager manager;
uint8_t sendVersionAmount = 0;
uint32_t t_sendversion;
uint32_t versionESP = 0;

void setup()
{
    // manager.initialize();

    // versionESP = (completeVersion[0] - 48) * 10000000L +
    //             (completeVersion[1] - 48) * 1000000L +
    //             (completeVersion[2] - 48) * 100000L +
    //             (completeVersion[3] - 48) * 10000L +
    //             (completeVersion[4] - 48) * 1000L +
    //             (completeVersion[5] - 48) * 100L +
    //             (completeVersion[6] - 48) * 10L +
    //             (completeVersion[7] - 48) * 1L;

    // t_sendversion = millis();
}

void loop()
{
    PuertoSerial puerto(115200);

    
    //manager.run();

    // if (sendVersionAmount < 3)
	// {
	// 	if (millis() - t_sendversion > 3000)
	// 	{
	// 		Serial.print("ESPV,");
	// 		Serial.print(versionESP);
	// 		Serial.print(",");
	// 		Serial.print('\0');
	// 		t_sendversion = millis();
	// 		sendVersionAmount++;
	// 	}
	// }
}

#endif