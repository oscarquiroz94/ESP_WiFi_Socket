#pragma once

#include "Compiletype.hpp"
#include "adapters/ESPadapter.hpp"
#include "CrossSectionalData.hpp"
#include "adapters/WebSocketAdapter.hpp"

extern WiFiClass WiFi;

class CheckSSID
{
    public:
        static void validateSSID(CrossSectionalDataEEPROM &eepromdata)
        {
            uint16_t numNetworks = WiFi.scanNetworks();

            for (int i = 0; i < numNetworks; i++)
            {
                String onNetworkSSID = WiFi.SSID(i);
                String currentSSID(eepromdata.ssidSocket);

                if (currentSSID == onNetworkSSID) assignAnotherSSID(eepromdata);
            }
        }

    private:
        static void assignAnotherSSID(CrossSectionalDataEEPROM &eepromdata)
        {
            size_t length    = strlen(eepromdata.ssidSocket); 
            size_t maxLength = sizeof(eepromdata.ssidSocket);

            // Check if two more characters can be added or then remove them
            if (length + 2 >= maxLength - 1)
            {
                eepromdata.ssidSocket[length - 2] = '\0'; 
            }

            char lastChar   = eepromdata.ssidSocket[length - 1];
            char penultChar = eepromdata.ssidSocket[length - 2];

            if (isNumericChar(lastChar) && penultChar == '_')
            {
                uint16_t newIdentifier = getNumericValue(lastChar) + 1;
                eepromdata.ssidSocket[length - 1] = '0' + newIdentifier;
            }
            else strcat(eepromdata.ssidSocket, "_1");
        }

        static bool isNumericChar(const char c)
        {
            return (c >= '0' && c <= '9');
        }

        static uint16_t getNumericValue(const char c)
        {
            if (isNumericChar(c))
            {
                return static_cast<uint16_t>(c - '0');
            }
            return 0;
        }
};
