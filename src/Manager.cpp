#include "Manager.hpp"
#include <algorithm>
#include "adapters/ESPadapter.hpp"
#include "utilities/CheckSSID.hpp"
#include "websocket/CredentialNotification.hpp"
#include "utilities/Version.h"

#ifdef DEPLOY
#include <EEPROM.h>
#endif


void Manager::initialize()
{
    versionESP = (completeVersion[0] - 48) * 10000000L +
                 (completeVersion[1] - 48) * 1000000L +
                 (completeVersion[2] - 48) * 100000L +
                 (completeVersion[3] - 48) * 10000L +
                 (completeVersion[4] - 48) * 1000L +
                 (completeVersion[5] - 48) * 100L +
                 (completeVersion[6] - 48) * 10L +
                 (completeVersion[7] - 48) * 1L;

    serialport.openPort();

    //eepromdata.read();
    
    registerSerialPortHandler();
    registerWebSocketHandler();

    webSocket.onEvent([&](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
        clientHandler.onWebSocketEvent(num, type, payload, length);
    });

}

void Manager::run()
{
    webSocket.loop();

    serialport.listen();
    serialport.processEvent();

    beat.loop();

    send_data();

    ESPadapter::retardo(5);
}

// TODO: TESTING
void Manager::send_data()
{
    if (ESPadapter::milliseconds() - t_sendversion > 3000 &&
        sendVersionAmount < 3)
    {
        ESPadapter::serial_print("ESPV,");
        ESPadapter::serial_print(versionESP);
        ESPadapter::serial_print(",");
        ESPadapter::serial_print('\0');
        t_sendversion = ESPadapter::milliseconds();
        sendVersionAmount++;
    }

    if (beat.is_alert() && !heartbeatonce)
    {
        ESPadapter::serial_print("HEARBEAT-DEAD");
        ESPadapter::serial_print('\0');
        heartbeatonce = true;
    }
}

void Manager::registerSerialPortHandler()
{
    serialport.addFunctionToMainCommand("S,", [&](const char* comand) { 
        CrossSectionalDataEEPROM newdata;

        char* cpycommand = (char*)comand;
        char *lista = strtok(cpycommand, ",");

        lista = strtok(NULL, ",");
        if (lista != NULL) strcpy(newdata.ssidSocket, String(lista).c_str());

        lista = strtok(NULL, ",");
        if (lista != NULL) strcpy(newdata.passSocket, String(lista).c_str());

        lista = strtok(NULL, ",");
        newdata.canalwifi = (uint8_t)ESPadapter::str2int(lista);

        CheckSSID::validateSSID(newdata);

        CredentialNotification::notifyOnChange(webSocket, clientHandler, newdata, eepromdata);

        bool sucess = WebsocketManager::buildWebSocket(webSocket, newdata);
        
        if (sucess) 
        {
            eepromdata = newdata;
            //eepromdata.save();

            IPAddress IP = WiFi.softAPIP();

            ESPadapter::serial_print("CSO");
            ESPadapter::serial_write('\0');

            ESPadapter::serial_print("IPS");
            ESPadapter::serial_print(IP.toString());
            ESPadapter::serial_print("CH");
            ESPadapter::serial_print(eepromdata.canalwifi);
            ESPadapter::serial_write('\0');

            ESPadapter::serial_print("SID,");
            ESPadapter::serial_print(eepromdata.ssidSocket);
            ESPadapter::serial_print(",");
            ESPadapter::serial_print(eepromdata.passSocket);
            ESPadapter::serial_print(",");
            ESPadapter::serial_print(eepromdata.canalwifi);
            ESPadapter::serial_print(",");
            ESPadapter::serial_write('\0');

            ESPadapter::flush();
        }
    });

    serialport.addFunctionToMainCommand("MCA", [&](const char* comand){
        ArtisanMessageStartRoasting msg;
        artisanClient.sendEvent(webSocket, &msg);
    });

    serialport.addFunctionToMainCommand("MDR", [&](const char* comand){
        ArtisanMessageEndRoasting msg;
        artisanClient.sendEvent(webSocket, &msg);
    });

    serialport.addFunctionToMainCommand("MFC", [&](const char* comand){
        ArtisanMessageFirstCrack msg;
        artisanClient.sendEvent(webSocket, &msg);
    });

    serialport.addFunctionToMainCommand("IN,", [&](const char* comand) { 
        char* cpycommand = (char*)comand;
        char *lista = strtok(cpycommand, ",");

		lista = strtok(NULL, ",");
		applicationdata.tempET = ESPadapter::str2int(lista); 

		lista = strtok(NULL, ",");
		applicationdata.tempBT = ESPadapter::str2int(lista);

		lista = strtok(NULL, ",");
		applicationdata.porcentQuem = ESPadapter::str2int(lista);;

		lista = strtok(NULL, ",");
		applicationdata.porcentTamb = ESPadapter::str2int(lista);;

		lista = strtok(NULL, ",");
		applicationdata.porcentSopl = ESPadapter::str2int(lista);;

		lista = strtok(NULL, ",");
		applicationdata.RoR = ESPadapter::str2int(lista);

		lista = strtok(NULL, ",");
		applicationdata.deltaETBT = ESPadapter::str2int(lista);
    });

    serialport.addFunctionToMainCommand("PAIR", [&](const char* comand){
        peer.executePairing(eepromdata);
    });

    //------------- Debug purposes ----------------

    serialport.addFunctionToMainCommand("STOPWS", [&](const char* comand){
        ESPadapter::serial_println("Stopping WebSocket server");
        WebsocketManager::destroyWebSocket(webSocket);
    });

    serialport.addFunctionToMainCommand("SHOW", [&](const char* comand){
        applicationdata.print();
    });

    serialport.addFunctionToMainCommand("LOG1", [&](const char* comand){
        ESPadapter::serial_println("Debug traces enabled");
        ESPadapter::trace_debug = true;
    });

    serialport.addFunctionToMainCommand("LOGNULL", [&](const char* comand){
        ESPadapter::serial_println("All traces disabled");
        ESPadapter::trace_debug = false;
    });
}

void Manager::registerWebSocketHandler()
{
    // Registrar Artisan siempre
    registerArtisan();

    // if (std::find(eepromdata.clientNames.begin(), eepromdata.clientNames.end(), "audiocrack") 
    //     != eepromdata.clientNames.end()) 
    //     registerAudioCrack();
}

void Manager::registerArtisan()
{
    clientHandler.registerWebsocketClient(artisanClient);

    artisanClient.addFunctionToMainCommand("getData", [&](uint8_t num, JsonDocument& doc) {
        std::string output;
        JsonDocument outdoc;
        outdoc["id"] = doc["id"];
        outdoc["data"]["aire"] = applicationdata.tempET;
        outdoc["data"]["grano"] = applicationdata.tempBT;
        outdoc["data"]["ror"] = applicationdata.RoR;
        outdoc["data"]["quemador"] = applicationdata.porcentQuem;
        outdoc["data"]["soplador"] = applicationdata.porcentSopl;
        outdoc["data"]["tambor"] = applicationdata.porcentTamb;
        outdoc["data"]["delta"] = applicationdata.deltaETBT;
        serializeJson(outdoc, output);
        webSocket.sendTXT(num, output);
        ESPadapter::debug_print("TO-ARTISAN: ");
        ESPadapter::debug_println(output);

        beat.set_status(Heartbeat::online);
    });

    artisanClient.addFunctionToMainCommand("setControlParams", [&](uint8_t num, JsonDocument& doc) {
        if (doc["params"]["aire"].is<int16_t>()) 
            applicationdata.aire = (int16_t)doc["params"]["aire"];

        if (doc["params"]["tambor"].is<int16_t>()) 
            applicationdata.tambor = (int16_t)doc["params"]["tambor"];

        if (doc["params"]["quemador"].is<int16_t>()) 
            applicationdata.quemador = (int16_t)doc["params"]["quemador"];

        if (doc["params"]["tedvalue"].is<int16_t>())
            applicationdata.tedvalue = (int16_t)doc["params"]["tedvalue"];

        ESPadapter::serial_print("PARAM,");
        ESPadapter::serial_print(applicationdata.aire);ESPadapter::serial_print(',');
        ESPadapter::serial_print(applicationdata.tambor);ESPadapter::serial_print(',');
        ESPadapter::serial_print(applicationdata.quemador);ESPadapter::serial_print(',');
        ESPadapter::serial_print(applicationdata.tedvalue);ESPadapter::serial_print(',');
        ESPadapter::serial_write('\0');
    });

    artisanClient.addFunctionToMainCommand("endRoasting", [&](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("SODROP");
        ESPadapter::serial_print('\0');
        beat.set_step(Heartbeat::other);
    });

    artisanClient.addFunctionToMainCommand("ready", [&](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("SREADY");
        ESPadapter::serial_print('\0');
        heartbeatonce = false;
    });

    artisanClient.addFunctionToMainCommand("noready", [&](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("SNOREA");
        ESPadapter::serial_print('\0');
        beat.set_step(Heartbeat::other);
        heartbeatonce = false;
    });

    artisanClient.addFunctionToMainCommand("identify", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("IDENTIFY");
        ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToMainCommand("noidentify", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("NOIDENTIFY");
        ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToMainCommand("getinit", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("GETINIT");
        ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToMainCommand("reset", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("RESET");
        ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToMainCommand("fcstart", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("FCSTART");
        ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToMainCommand("oncharge", [&](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("ONCHARGE");
        ESPadapter::serial_print('\0');
        beat.set_step(Heartbeat::roasting);
    });

    artisanClient.addFunctionToMainCommand("onted", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("ONTED");
        ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToMainCommand("offted", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("OFFTED");
        ESPadapter::serial_print('\0');
    });
}

void Manager::registerAudioCrack()
{
    ESPadapter::debug_println("Registering AudioCrack client...");
    //clientHandler.registerWebsocketClient(audioCrackClient);

    // audioCrackClient.addFunctionToMainCommand("getData", [&](uint8_t num, JsonDocument& doc) {
    //     String output;
    //     JsonDocument outdoc;
    //     outdoc["id"] = doc["id"];
    //     outdoc["data"]["event"] = "firstCrackBeginningEvent";
    //     serializeJson(outdoc, output);
    //     webSocket.sendTXT(num, output);
    // });
}
