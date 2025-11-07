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
    registerWebSocketHandler();

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
            ESPadapter::retardo(50);
        }
    });

    serialport.addFunctionToMainCommand("MCA", [&](const char* comand){

        VisualScopeMessageStartRoasting scopeMsg;
        visualScopeClient.sendEvent(webSocket, &scopeMsg);

        AudioCrackMessageStartRoasting audioMsg;
        audioCrackClient.sendEvent(webSocket, &audioMsg);
    });

    serialport.addFunctionToMainCommand("MDR", [&](const char* comand){

        VisualScopeMessageEndRoasting scopeMsg;
        visualScopeClient.sendEvent(webSocket, &scopeMsg);

        AudioCrackMessageEndRoasting audioMsg;
        audioCrackClient.sendEvent(webSocket, &audioMsg);
    });

    serialport.addFunctionToMainCommand("MFC", [&](const char* comand){

        VisualScopeMessageFirstCrack scopeMsg;
        visualScopeClient.sendEvent(webSocket, &scopeMsg);

        AudioCrackMessageFirstCrack audioMsg;
        audioCrackClient.sendEvent(webSocket, &audioMsg);
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

        clientHandler.unregisterWebsocketClient(visualScopeClient);
        clientHandler.unregisterWebsocketClient(audioCrackClient);
        ESPadapter::debug_println("WBS clients: " + String(clientHandler.getClientCount()));
        peer.executePairing(eepromdata);
        registerWebSocketHandler();
        ESPadapter::debug_println("WBS clients: " + String(clientHandler.getClientCount()));
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

    serialport.addFunctionToMainCommand("MEMOFREE", [&](const char* comand){
        size_t totalHeap = ESP.getHeapSize();
        size_t freeHeap = ESP.getFreeHeap();
        size_t usedHeap = totalHeap - freeHeap;

        ESPadapter::serial_print("Memoria total (heap): ");
        ESPadapter::serial_println(totalHeap);

        ESPadapter::serial_print("Memoria usada (heap): ");
        ESPadapter::serial_println(usedHeap);

        ESPadapter::serial_print("Memoria libre (heap): ");
        ESPadapter::serial_println(freeHeap);
    });
}

void Manager::registerWebSocketHandler()
{
    // Se registran todos los posibles clientes
    registerVisualScope();
    registerAudioCrack();
}

void Manager::registerVisualScope()
{
    ESPadapter::debug_println("Registering VisualScope client...");

    visualScopeClient.setName("visualscope");
    clientHandler.registerWebsocketClient(visualScopeClient);

    visualScopeClient.addFunctionToMainCommand("getData", [&](uint8_t num, JsonDocument& doc) {

        //! No funciona, why ?
        // VisualScopeMessageOperatives scopeMsg;
        // scopeMsg.m_id = static_cast<int8_t>(doc["id"]);
        // scopeMsg.m_tempET = applicationdata.tempET;
        // scopeMsg.m_tempBT = applicationdata.tempBT;
        // scopeMsg.m_ror = applicationdata.RoR;
        // scopeMsg.m_porcentQuem = applicationdata.porcentQuem;
        // scopeMsg.m_porcentSopl = applicationdata.porcentSopl;
        // scopeMsg.m_porcentTamb = applicationdata.porcentTamb;
        // scopeMsg.m_deltaETBT = applicationdata.deltaETBT;
        // visualScopeClient.sendEvent(webSocket, &scopeMsg);

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

        AudioCrackMessageOperatives audioMsg;
        audioMsg.m_beanTemperature = applicationdata.tempBT;
        audioMsg.m_rateOfRise = applicationdata.RoR;
        audioCrackClient.sendEvent(webSocket, &audioMsg);

        beat.set_status(Heartbeat::online);
    });

    visualScopeClient.addFunctionToMainCommand("setControlParams", [&](uint8_t num, JsonDocument& doc) {

        applicationdata.aire = doc["params"]["aire"].as<int16_t>();
        applicationdata.tambor = (int16_t)doc["params"]["tambor"].as<int16_t>();
        applicationdata.quemador = (int16_t)doc["params"]["quemador"].as<int16_t>();
        applicationdata.tedvalue = (int16_t)doc["params"]["tedvalue"].as<int16_t>();

        ESPadapter::serial_print("PARAM,");
        ESPadapter::serial_print(applicationdata.aire);ESPadapter::serial_print(',');
        ESPadapter::serial_print(applicationdata.tambor);ESPadapter::serial_print(',');
        ESPadapter::serial_print(applicationdata.quemador);ESPadapter::serial_print(',');
        ESPadapter::serial_print(applicationdata.tedvalue);ESPadapter::serial_print(',');
        ESPadapter::serial_write('\0');
        ESPadapter::retardo(50);
    });

    visualScopeClient.addFunctionToMainCommand("endRoasting", [&](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("SODROP");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
        beat.set_step(Heartbeat::other);
    });

    visualScopeClient.addFunctionToMainCommand("ready", [&](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("SREADY");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
        heartbeatonce = false;
    });

    visualScopeClient.addFunctionToMainCommand("noready", [&](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("SNOREA");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
        beat.set_step(Heartbeat::other);
        heartbeatonce = false;
    });

    visualScopeClient.addFunctionToMainCommand("identify", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("IDENTIFY");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
    });

    visualScopeClient.addFunctionToMainCommand("noidentify", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("NOIDENTIFY");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
    });

    visualScopeClient.addFunctionToMainCommand("getinit", [&](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("GETINIT");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
    });

    visualScopeClient.addFunctionToMainCommand("reset", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("RESET");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
    });

    visualScopeClient.addFunctionToMainCommand("fcstart", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("FCSTART");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
    });

    visualScopeClient.addFunctionToMainCommand("oncharge", [&](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("ONCHARGE");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
        beat.set_step(Heartbeat::roasting);
    });

    visualScopeClient.addFunctionToMainCommand("onted", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("ONTED");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
    });

    visualScopeClient.addFunctionToMainCommand("offted", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("OFFTED");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
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
        // TODO: crear mensaje para esto
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
