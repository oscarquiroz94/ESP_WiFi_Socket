#include "Manager.hpp"
#include <algorithm>
#include "adapters/ESPadapter.hpp"
#include "utilities/CheckSSID.hpp"
#include "websocket/CredentialNotification.hpp"
#include "utilities/Version.h"

#ifdef DEPLOY
#include <EEPROM.h>
#endif

Manager::Manager() : 
    serialport(115200) 
    ,webSocket(8080)
    ,clientHandler(webSocket)
    ,visualScopeClient(&visualScopeMsg)
    ,audioCrackClient(&audioCrackMsg)
    ,peer(webSocket, clientHandler)
    {}

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
    
    registerSerialPortHandler();
    registerWebSocketHandler(); //! Comment for testing pairing

    webSocket.onEvent([&](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
        clientHandler.onWebSocketEvent(num, type, payload, length);
    });

    eepromdata.read();

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

    // if (sendVersionAmount == 3)
    // {
    //     peer.executePairing(eepromdata);
    //     sendVersionAmount++; // to avoid re-entering here
    //     registerWebSocketHandler();
    //     ESPadapter::debug_println("WBS clients: " + String(clientHandler.getClientCount()));
    // }
        
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

        bool sucess = WebsocketManager::turnOnWebSocket(webSocket, newdata);
        
        if (sucess) 
        {
            eepromdata = newdata;
            eepromdata.save();

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
        std::string output;
        JsonDocument outdoc;

        outdoc["pushMessage"] = "startRoasting";
        outdoc["command"] = "startroasting"; //! Temporal
        serializeJson(outdoc, output);
        webSocket.broadcastTXT(output.c_str());
        // VisualScopeMessageStartRoasting scopeMsg;
        // visualScopeClient.sendEvent(webSocket, &scopeMsg);

        //todo: id esta mal, no lo puedo asignar yo
        // AudioCrackMessageStartRoasting audioMsg;
        // audioCrackClient.sendEvent(webSocket, &audioMsg);
    });

    serialport.addFunctionToMainCommand("MDR", [&](const char* comand){
        std::string output;
        JsonDocument outdoc;

        outdoc["pushMessage"] = "endRoasting";
        outdoc["command"] = "endroasting"; //! Temporal
        this->firstCrack = false;
        serializeJson(outdoc, output);
        webSocket.broadcastTXT(output.c_str());
        // VisualScopeMessageEndRoasting msg;
        // visualScopeClient.sendEvent(webSocket, &msg);
    });

    serialport.addFunctionToMainCommand("MFC", [&](const char* comand){
        std::string output;
        JsonDocument outdoc;

        outdoc["pushMessage"] = "addEvent";
        outdoc["data"]["event"] = "firstCrackBeginningEvent";
        outdoc["command"] = "firstcrack"; //! Temporal
        this->firstCrack = true;
        serializeJson(outdoc, output);
        webSocket.broadcastTXT(output.c_str());
        // VisualScopeMessageFirstCrack msg;
        // visualScopeClient.sendEvent(webSocket, &msg);
    });

    serialport.addFunctionToMainCommand("IN,", [&](const char* comand) { 
        char* cpycommand = (char*)comand;
        char *lista = strtok(cpycommand, ",");

		lista = strtok(NULL, ",");
		applicationdata.tempET = ESPadapter::str2int(lista); 

		lista = strtok(NULL, ",");
		applicationdata.tempBT = ESPadapter::str2int(lista);

		lista = strtok(NULL, ",");
		applicationdata.porcentQuem = ESPadapter::str2int(lista);

		lista = strtok(NULL, ",");
		applicationdata.porcentTamb = ESPadapter::str2int(lista);

		lista = strtok(NULL, ",");
		applicationdata.porcentSopl = ESPadapter::str2int(lista);

		lista = strtok(NULL, ",");
		applicationdata.RoR = ESPadapter::str2int(lista);

		lista = strtok(NULL, ",");
		applicationdata.deltaETBT = ESPadapter::str2int(lista);

        AudioCrackMessageOperatives msg;
        msg.m_beanTemperature = applicationdata.tempBT;
        msg.m_rateOfRise = applicationdata.RoR;
        audioCrackClient.sendEvent(webSocket, &msg);
    });

    // This will be requested by the ATM
    serialport.addFunctionToMainCommand("PAIR", [&](const char* comand){
        peer.executePairing(eepromdata);
    });

    //------------- Debug purposes ----------------

    serialport.addFunctionToMainCommand("STOPWS", [&](const char* comand){
        ESPadapter::serial_println("Stopping WebSocket server");
        WebsocketManager::turnOffWebSocket(webSocket);
    });

    serialport.addFunctionToMainCommand("SHOW", [&](const char* comand){
        applicationdata.print();
        eepromdata.print();
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
    // Registrar VisualScope siempre
    registerVisualScope();

    /*
      if (std::find(eepromdata.clientNames.begin(), eepromdata.clientNames.end(), "audiocrack") 
        != eepromdata.clientNames.end()) 
        registerAudioCrack();*/
    registerAudioCrack();
    
}

void Manager::registerVisualScope()
{
    ESPadapter::debug_println("Registering VisualScope client...");

    visualScopeClient.setName("visualscope");
    clientHandler.registerWebsocketClient(visualScopeClient);

    visualScopeClient.addFunctionToMainCommand("getData", [&](uint8_t num, JsonDocument& doc) {
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

        //! Temporal
        outdoc["command"] = "operatives";
        outdoc["data"]["beantemperature"] = applicationdata.tempBT;
        outdoc["data"]["rateofrise"] = applicationdata.RoR;
        outdoc["data"]["manualfccrack"] = this->firstCrack;
        //!------------

        serializeJson(outdoc, output);

        
        //webSocket.sendTXT(num, output);
        webSocket.broadcastTXT(output.c_str()); //! Temporal
    
        ESPadapter::debug_print("TO-ARTISAN: ");
        ESPadapter::debug_println(output);


        beat.set_status(Heartbeat::online);
    });

    visualScopeClient.addFunctionToMainCommand("setControlParams", [&](uint8_t num, JsonDocument& doc) {
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

    visualScopeClient.addFunctionToMainCommand("endRoasting", [&](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("SODROP");
        ESPadapter::serial_print('\0');
        beat.set_step(Heartbeat::other);
    });

    visualScopeClient.addFunctionToMainCommand("ready", [&](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("SREADY");
        ESPadapter::serial_print('\0');
        heartbeatonce = false;
    });

    visualScopeClient.addFunctionToMainCommand("noready", [&](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("SNOREA");
        ESPadapter::serial_print('\0');
        beat.set_step(Heartbeat::other);
        heartbeatonce = false;
    });

    visualScopeClient.addFunctionToMainCommand("identify", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("IDENTIFY");
        ESPadapter::serial_print('\0');
    });

    visualScopeClient.addFunctionToMainCommand("noidentify", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("NOIDENTIFY");
        ESPadapter::serial_print('\0');
    });

    visualScopeClient.addFunctionToMainCommand("getinit", [&](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("GETINIT");
        ESPadapter::serial_print('\0');
    });

    visualScopeClient.addFunctionToMainCommand("reset", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("RESET");
        ESPadapter::serial_print('\0');
    });

    visualScopeClient.addFunctionToMainCommand("fcstart", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("FCSTART");
        ESPadapter::serial_print('\0');
    });

    visualScopeClient.addFunctionToMainCommand("oncharge", [&](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("ONCHARGE");
        ESPadapter::serial_print('\0');
        beat.set_step(Heartbeat::roasting);
    });

    visualScopeClient.addFunctionToMainCommand("onted", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("ONTED");
        ESPadapter::serial_print('\0');
    });

    visualScopeClient.addFunctionToMainCommand("offted", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("OFFTED");
        ESPadapter::serial_print('\0');
    });
}

void Manager::registerAudioCrack()
{
    ESPadapter::debug_println("Registering AudioCrack client...");
    
    audioCrackClient.setName("audiocrack");
    clientHandler.registerWebsocketClient(audioCrackClient);

    audioCrackClient.addFunctionToMainCommand("getinitial", [&](uint8_t num, JsonDocument& doc) {
        // Send initial data to audiocrack device
        // {
        // "command":"config",
        // "data":
        //       {
        //         "configuracion1":150,
        //         "configuracion2":20
        //       }
        // }
        std::string output;
        JsonDocument outdoc;
        outdoc["command"] = "config";
        outdoc["data"]["configuracion1"] = 150;
        outdoc["data"]["configuracion2"] = 20;

        serializeJson(outdoc, output);
        webSocket.sendTXT(num, output);
        ESPadapter::debug_print("TO-AUDIOCRACK: ");
        ESPadapter::debug_println(output);
    });

    audioCrackClient.addFunctionToMainCommand("firstcrack", [&](uint8_t num, JsonDocument& doc) {
        // Audiocrack device notification for first crack event
    });
}
