#pragma once

#include "adapters/ESPadapter.hpp"
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

    void print()
    {
        ESPadapter::serial_print("Tempera ET: "); ESPadapter::serial_println(tempET);
        ESPadapter::serial_print("Tempera BT: "); ESPadapter::serial_println(tempBT);
        ESPadapter::serial_print("Roaster quem: "); ESPadapter::serial_println(porcentQuem);
        ESPadapter::serial_print("Roaster tamb: "); ESPadapter::serial_println(porcentTamb);
        ESPadapter::serial_print("Roaster sopla: "); ESPadapter::serial_println(porcentSopl);
        ESPadapter::serial_print("RoR: "); ESPadapter::serial_println(RoR);
        ESPadapter::serial_print("Artisan quem: "); ESPadapter::serial_println(aire);
        ESPadapter::serial_print("Artisan tambor: "); ESPadapter::serial_println(tambor);
        ESPadapter::serial_print("Artisan quemador: "); ESPadapter::serial_println(quemador);
    }
};

struct CrossSectionalDataEEPROM
{
    char ssidSocket[20] = "ROASTER";
    char passSocket[20] = "Clave123*";
    uint8_t canalwifi = 1;
    std::vector<std::string> clientNames; // it should not be here

#ifdef DEPLOY
    Preferences preferences;
#endif

    void save()
    {
#ifdef DEPLOY
        preferences.begin("config", false);

        String oldSSID = preferences.getString("ssidSocket", "");
        if (oldSSID != ssidSocket) preferences.putString("ssidSocket", ssidSocket);

        String oldPass = preferences.getString("passSocket", "");
        if (oldPass != passSocket) preferences.putString("passSocket", passSocket);

        uint8_t oldCanalWifi = preferences.getUChar("canalwifi", 1);
        if (oldCanalWifi != canalwifi) preferences.putUChar("canalwifi", canalwifi);
        
        // std::string concatNames = "";
        // for (std::vector<std::string>::iterator it = clientNames.begin(); it != clientNames.end(); ++it) 
        // {
        //     concatNames += *it + ",";
        // }
        // preferences.putString("clientNames", concatNames.c_str());

        preferences.end();
#endif
        ESPadapter::debug_println("Preferences: updated");
    }

    void read()
    {
#ifdef DEPLOY
        memset(ssidSocket, 0, sizeof(ssidSocket));
        memset(passSocket, 0, sizeof(passSocket));

        preferences.begin("config", true);
        preferences.getString("ssidSocket", ssidSocket, sizeof(ssidSocket));
        preferences.getString("passSocket", passSocket, sizeof(passSocket));
        canalwifi = preferences.getUChar("canalwifi", 1);
        //preferences.getString("clientNames", _clientNames, sizeof(_clientNames));
        preferences.end();

        print();

        // ESPadapter::debug_print("SSID: ");
        // ESPadapter::debug_println(ssidSocket);
        // ESPadapter::debug_print("Pass: ");
        // ESPadapter::debug_println(passSocket);
        // ESPadapter::debug_print("Canal wifi: ");
        // ESPadapter::debug_println(canalwifi);

        // ESPadapter::debug_print("Client names: ");
        // char* token = strtok(_clientNames, ",");
        // while (token != nullptr) 
        // {
        //     if (strlen(token) > 0) 
        //     {
        //         ESPadapter::debug_print(token);
        //         ESPadapter::debug_print(F(","));
        //         // clientNames.push_back(std::string(token));
        //     }
        //     token = strtok(nullptr, ",");
        // }
#endif
    }

    void print()
    {
        ESPadapter::debug_println("===================================");
        ESPadapter::debug_print("Preference SSID: "); ESPadapter::debug_println(ssidSocket);
        ESPadapter::debug_print("Preference Pass: "); ESPadapter::debug_println(passSocket);
        ESPadapter::debug_print("Preference Canal wifi: "); ESPadapter::debug_println(canalwifi);
        ESPadapter::debug_print("Preference Client names: ");
        for (std::vector<std::string>::iterator it = clientNames.begin(); it != clientNames.end(); ++it) 
        {
            const char* name = it->c_str();
            ESPadapter::debug_print(name);
            ESPadapter::debug_print(",");
        }
        ESPadapter::debug_println();
        ESPadapter::debug_println("===================================");
    }
};
