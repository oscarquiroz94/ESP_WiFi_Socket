#include "CrossSectionalData.hpp"

void CrossSectionalData::print()
{
    ESPadapter::serial_print("Tempera ET: "); ESPadapter::serial_println(tempET);
    ESPadapter::serial_print("Tempera BT: "); ESPadapter::serial_println(tempBT);
    ESPadapter::serial_print("Roaster quem: "); ESPadapter::serial_println(porcentQuem);
    ESPadapter::serial_print("Roaster tamb: "); ESPadapter::serial_println(porcentTamb);
    ESPadapter::serial_print("Roaster sopla: "); ESPadapter::serial_println(porcentSopl);
    ESPadapter::serial_print("RoR: "); ESPadapter::serial_println(RoR);
    ESPadapter::serial_print("VisualScope quem: "); ESPadapter::serial_println(aire);
    ESPadapter::serial_print("VisualScope tambor: "); ESPadapter::serial_println(tambor);
    ESPadapter::serial_print("VisualScope quemador: "); ESPadapter::serial_println(quemador);
}


void CrossSectionalDataEEPROM::save()
{
#ifdef DEPLOY
    preferences.begin("config", false);

    String oldSSID = preferences.getString("ssidSocket", "");
    if (oldSSID != ssidSocket) preferences.putString("ssidSocket", ssidSocket);

    String oldPass = preferences.getString("passSocket", "");
    if (oldPass != passSocket) preferences.putString("passSocket", passSocket);

    uint8_t oldCanalWifi = preferences.getUChar("canalwifi", 1);
    if (oldCanalWifi != canalwifi) preferences.putUChar("canalwifi", canalwifi);

    preferences.end();
#endif
    ESPadapter::debug_println("Preferences: updated");
}

void CrossSectionalDataEEPROM::read()
{
#ifdef DEPLOY
    memset(ssidSocket, 0, sizeof(ssidSocket));
    memset(passSocket, 0, sizeof(passSocket));

    preferences.begin("config", true);
    preferences.getString("ssidSocket", ssidSocket, sizeof(ssidSocket));
    preferences.getString("passSocket", passSocket, sizeof(passSocket));
    canalwifi = preferences.getUChar("canalwifi", 1);
    preferences.end();

    print();

#endif
}

void CrossSectionalDataEEPROM::print()
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