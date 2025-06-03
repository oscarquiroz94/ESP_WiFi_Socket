#include "Manager.hpp"
#include "adapters/ESPadapter.hpp"
#include "utilities/CheckSSID.hpp"
#include <algorithm>

#ifdef DEPLOY
#include <EEPROM.h>
#endif


void Manager::initialize()
{
    serialport.openPort();

    eepromdata.read();
    
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

    ESPadapter::retardo(5);
}

void Manager::sendInitializationData(const uint32_t version)
{
    if (ESPadapter::milliseconds() - t_sendversion > 3000 &&
        sendVersionAmount < 3)
    {
        serialport.sendRawData("ESPV,");
        serialport.sendRawData(version);
        serialport.sendRawData(",");
        serialport.sendRawData('\0');
        t_sendversion = ESPadapter::milliseconds();
        sendVersionAmount++;
    }
}

void Manager::registerSerialPortHandler()
{
    serialport.addFunctionToMainCommand("S,", [&](const char* comand) { 
        //Limpiar valores anteriores
        memset(eepromdata.ssidSocket, 0, sizeof(eepromdata.ssidSocket));
        memset(eepromdata.passSocket, 0, sizeof(eepromdata.passSocket));

        char* cpycommand = (char*)comand;
        char *lista = strtok(cpycommand, ",");

        lista = strtok(NULL, ",");
        if (lista != NULL) strcpy(eepromdata.ssidSocket, String(lista).c_str());

        lista = strtok(NULL, ",");
        if (lista != NULL) strcpy(eepromdata.passSocket, String(lista).c_str());

        lista = strtok(NULL, ",");
        eepromdata.canalwifi = (uint8_t)ESPadapter::str2int(lista);

        peer.executePairing(eepromdata);

        CheckSSID::validateSSID(eepromdata);

        bool sucess = WebsocketManager::buildWebSocket(webSocket, eepromdata);
        
        if (sucess) 
        {
            eepromdata.save();

            IPAddress IP = WiFi.softAPIP();

            ESPadapter::serial_print("CSO"); // \0 included at the end

            ESPadapter::serial_print(
                std::string("IPS") + IP.toString() 
                + std::string("CH") + std::to_string(eepromdata.canalwifi)
            );

            ESPadapter::serial_print(
                std::string("SID,") + eepromdata.ssidSocket + ","
                + eepromdata.passSocket + ","
                + std::to_string(eepromdata.canalwifi)
            );

            ESPadapter::flush();
        }
    });

    serialport.addFunctionToMainCommand("MCA", [&](const char* comand){
        std::string output;
        JsonDocument outdoc;
        int8_t idArtisan = artisanClient.getClientId();

        outdoc["pushMessage"] = "startRoasting";
        serializeJson(outdoc, output);
        webSocket.sendTXT(idArtisan, output);
    });

    serialport.addFunctionToMainCommand("MDR", [&](const char* comand){
        std::string output;
        JsonDocument outdoc;
        int8_t idArtisan = artisanClient.getClientId();

        outdoc["pushMessage"] = "endRoasting";
        serializeJson(outdoc, output);
        webSocket.sendTXT(idArtisan, output);
    });

    serialport.addFunctionToMainCommand("MFC", [&](const char* comand){
        std::string output;
        JsonDocument outdoc;
        int8_t idArtisan = artisanClient.getClientId();

        outdoc["pushMessage"] = "addEvent";
		outdoc["data"]["event"] = "firstCrackBeginningEvent";
        serializeJson(outdoc, output);
        webSocket.sendTXT(idArtisan, output);
    });

    // IN,230,160,1500,2000,200,600,200 --> ET, BT, Q, T, S, ROR, delta
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
		applicationdata.deltaETBT = ESPadapter::str2int(lista);;
    });

    //------------- Debug purposes ----------------

    serialport.addFunctionToMainCommand("STOPWS", [&](const char* comand){
        ESPadapter::serial_println("Stopping WebSocket server");
        WebsocketManager::destroyWebSocket(webSocket);
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

    if (std::find(eepromdata.clientNames.begin(), eepromdata.clientNames.end(), "audiocrack") 
        != eepromdata.clientNames.end()) 
        registerAudioCrack();
}

void Manager::registerArtisan()
{
    clientHandler.registerWebsocketClient(artisanClient);

    artisanClient.addFunctionToMainCommand("getData", [&](uint8_t num, JsonDocument& doc) {
        std::string output;
        JsonDocument outdoc;
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
        ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToMainCommand("endRoasting", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("SODROP");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToMainCommand("ready", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("SREADY");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToMainCommand("noready", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("SNOREA");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToMainCommand("identify", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("IDENTIFY");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToMainCommand("noidentify", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("NOIDENTIFY");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToMainCommand("getinit", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("GETINIT");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToMainCommand("reset", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("RESET");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToMainCommand("fcstart", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("FCSTART");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToMainCommand("oncharge", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("ONCHARGE");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToMainCommand("onted", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("ONTED");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToMainCommand("offted", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("OFFTED");ESPadapter::serial_print('\0');
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
