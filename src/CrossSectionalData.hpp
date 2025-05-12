#pragma once

#include "ESPadapter.hpp"

struct CrossSectionalData
{
    char ssidSocket[50] = "ROASTER";
    char passSocket[50] = "Clave123";
    uint8_t canalwifi = 1;

    int16_t tempET = 0;
    int16_t tempBT = 0;
    int16_t porcentQuem = 0;
    int16_t porcentTamb = 0;
    int16_t porcentSopl = 0;
    int16_t RoR = 0;
    int16_t deltaETBT = 0;

    int16_t aire = 0;
    int16_t tambor = 0;
    int16_t quemador = 0;
    int16_t tedvalue = 0;
};
