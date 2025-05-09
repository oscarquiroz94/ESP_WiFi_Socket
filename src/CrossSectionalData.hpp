#pragma once

#include "ESPadapter.hpp"

struct CrossSectionalData
{
    char ssidSocket[50] = "ROASTER";
    char passSocket[50] = "Clave123";
    uint8_t canalwifi = 1;
    double tempET = 0.0;
    double tempBT = 0.0;
    double porcentQuem = 0.0;
    double porcentTamb = 0.0;
    double porcentSopl = 0.0;
    double RoR = 0.0;
    double deltaETBT = 0.0;
};
