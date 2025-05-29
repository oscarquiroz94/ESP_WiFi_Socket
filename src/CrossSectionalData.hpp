#pragma once

#include "utilities/ESPadapter.hpp"
#include "Compiletype.hpp"
#include <vector>
#include <array>
#include <iostream>

#ifdef DEPLOY
#include <Preferences.h>
#endif


struct CrossSectionalData
{
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

struct CrossSectionalDataEEPROM
{
    char ssidSocket[20] = "ROASTER";
    char passSocket[20] = "Clave123*";
    uint8_t canalwifi = 1;
    std::vector<std::string> clientNames;

#ifdef DEPLOY
    Preferences preferences;
#endif

    void save()
    {
#ifdef DEPLOY
        preferences.begin("config", false);
        preferences.putString("ssidSocket", ssidSocket);
        preferences.putString("passSocket", passSocket);
        preferences.putUChar("canalwifi", canalwifi);
        
        std::string concatNames = "";
        for (std::vector<std::string>::iterator it = clientNames.begin(); it != clientNames.end(); ++it) 
        {
            concatNames += *it + ",";
        }
        preferences.putString("clientNames", concatNames.c_str());

        preferences.end();
#endif
        ESPadapter::serial_println("EEPROM: updated");
    }

    void read()
    {
#ifdef DEPLOY
        char _ssidSocket[20];
        char _passSocket[20];
        char _clientNames[100];
        uint8_t _canalwifi;

        preferences.begin("config", true);
        preferences.getString("ssidSocket", _ssidSocket, sizeof(_ssidSocket));
        preferences.getString("passSocket", _passSocket, sizeof(_passSocket));
        _canalwifi = preferences.getUChar("canalwifi", 1);
        preferences.getString("clientNames", _clientNames, sizeof(_clientNames));
        preferences.end();

        ESPadapter::serial_print("SSID: ");
        ESPadapter::serial_println(_ssidSocket);
        ESPadapter::serial_print("Pass: ");
        ESPadapter::serial_println(_passSocket);
        ESPadapter::serial_print("Canal wifi: ");
        ESPadapter::serial_println(_canalwifi);


        ESPadapter::serial_print("Client names: ");
        char* token = strtok(_clientNames, ",");
        while (token != nullptr) 
        {
            if (strlen(token) > 0) 
            {
                ESPadapter::serial_print(token);
                ESPadapter::serial_print(F(","));
                // clientNames.push_back(std::string(token));
            }
            token = strtok(nullptr, ",");
        }
#endif
    }

    void print()
    {
        ESPadapter::serial_println("===================================");
        ESPadapter::serial_print("SSID: "); ESPadapter::serial_println(ssidSocket);
        ESPadapter::serial_print("Pass: "); ESPadapter::serial_println(passSocket);
        ESPadapter::serial_print("Canal wifi: "); ESPadapter::serial_println(canalwifi);
        ESPadapter::serial_print("Client names: ");
        for (std::vector<std::string>::iterator it = clientNames.begin(); it != clientNames.end(); ++it) 
        {
            const char* name = it->c_str();
            ESPadapter::serial_print(name);
            ESPadapter::serial_print(",");
        }
        ESPadapter::serial_println();
        ESPadapter::serial_println("===================================");
    }
};
