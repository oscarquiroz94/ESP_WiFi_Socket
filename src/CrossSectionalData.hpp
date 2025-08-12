#pragma once

#include "adapters/ESPadapter.hpp"
#include "Compiletype.hpp"
#include <vector>
#include <array>
#include <iostream>

#ifdef DEPLOY
#include <Preferences.h>
#endif


class CrossSectionalData
{
    public:
        int16_t tempET      {0};
        int16_t tempBT      {0};
        int16_t porcentQuem {0};
        int16_t porcentTamb {0};
        int16_t porcentSopl {0};
        int16_t RoR         {0};
        int16_t deltaETBT   {0};
        int16_t aire        {0};
        int16_t tambor      {0};
        int16_t quemador    {0};
        int16_t tedvalue    {0};

        void print();
};

class CrossSectionalDataEEPROM
{
    public:
        char ssidSocket[20] = "ROASTER";
        char passSocket[20] = "Clave123*";
        uint8_t canalwifi = 1;
        std::vector<std::string> clientNames; // it should not be here

#ifdef DEPLOY
        Preferences preferences;
#endif

        void save();
        void read();
        void print();

        bool operator==(const CrossSectionalDataEEPROM& other) const
        {
            return strcmp(ssidSocket, other.ssidSocket) == 0 &&
                    strcmp(passSocket, other.passSocket) == 0 &&
                    canalwifi == other.canalwifi;
        }
};
