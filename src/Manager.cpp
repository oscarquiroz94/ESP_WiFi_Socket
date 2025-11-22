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
    m_serialport(115200) 
    ,m_webSocket(8080)
    ,m_clientHandler(m_webSocket)
    ,m_visualScopeClient(&m_visualScopeMsg)
    ,m_audioCrackClient(&m_audioCrackMsg)
    ,m_peer(m_webSocket, m_clientHandler)
    {}

void Manager::initialize()
{
    m_versionESP = (completeVersion[0] - 48) * 10000000L +
                 (completeVersion[1] - 48) * 1000000L +
                 (completeVersion[2] - 48) * 100000L +
                 (completeVersion[3] - 48) * 10000L +
                 (completeVersion[4] - 48) * 1000L +
                 (completeVersion[5] - 48) * 100L +
                 (completeVersion[6] - 48) * 10L +
                 (completeVersion[7] - 48) * 1L;

    m_serialport.openPort();
    
    registerSerialPortHandler();
    registerWebSocketHandler();

    m_webSocket.onEvent([&](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
        m_clientHandler.onWebSocketEvent(num, type, payload, length);
    });

    m_eepromdata.read();

}

void Manager::run()
{
    m_webSocket.loop();

    m_serialport.listen();
    m_serialport.processEvent();

    m_beat.loop();

    send_data();

    ESPadapter::retardo(5);
}

void Manager::send_data()
{
    if (ESPadapter::milliseconds() - m_tempo_sendversion > 3000 &&
        m_sendVersionAmount < 3)
    {
        ESPadapter::serial_print("ESPV,");
        ESPadapter::serial_print(m_versionESP);
        ESPadapter::serial_print(",");
        ESPadapter::serial_print('\0');
        m_tempo_sendversion = ESPadapter::milliseconds();
        m_sendVersionAmount++;
    }

    if (m_beat.is_alert() && !m_heartbeatonce)
    {
        ESPadapter::serial_print("HEARBEAT-DEAD");
        ESPadapter::serial_print('\0');
        m_heartbeatonce = true;
    }
        
}

void Manager::registerSerialPortHandler()
{
    m_serialport.addFunctionToMainCommand("S,", [&](const char* comand) { 
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

        CredentialNotification::notifyOnChange(m_webSocket, m_clientHandler, newdata, m_eepromdata);

        bool sucess = WebsocketManager::turnOnWebSocket(m_webSocket, newdata);
        
        if (sucess) 
        {
            m_eepromdata = newdata;
            m_eepromdata.save();

            IPAddress IP = WiFi.softAPIP();

            ESPadapter::serial_print("CSO");
            ESPadapter::serial_write('\0');

            ESPadapter::serial_print("IPS");
            ESPadapter::serial_print(IP.toString());
            ESPadapter::serial_print("CH");
            ESPadapter::serial_print(m_eepromdata.canalwifi);
            ESPadapter::serial_write('\0');

            ESPadapter::serial_print("SID,");
            ESPadapter::serial_print(m_eepromdata.ssidSocket);
            ESPadapter::serial_print(",");
            ESPadapter::serial_print(m_eepromdata.passSocket);
            ESPadapter::serial_print(",");
            ESPadapter::serial_print(m_eepromdata.canalwifi);
            ESPadapter::serial_print(",");
            ESPadapter::serial_write('\0');

            ESPadapter::flush();
            ESPadapter::retardo(50);
        }
    });

    m_serialport.addFunctionToMainCommand("MCA", [&](const char* comand){

        VisualScopeMessageStartRoasting scopeMsg;
        m_visualScopeClient.sendEvent(m_webSocket, &scopeMsg);

        AudioCrackMessageStartRoasting audioMsg;
        m_audioCrackClient.sendEvent(m_webSocket, &audioMsg);
    });

    m_serialport.addFunctionToMainCommand("MDR", [&](const char* comand){

        VisualScopeMessageEndRoasting scopeMsg;
        m_visualScopeClient.sendEvent(m_webSocket, &scopeMsg);

        AudioCrackMessageEndRoasting audioMsg;
        m_audioCrackClient.sendEvent(m_webSocket, &audioMsg);
    });

    m_serialport.addFunctionToMainCommand("MFC", [&](const char* comand){

        VisualScopeMessageFirstCrack scopeMsg;
        m_visualScopeClient.sendEvent(m_webSocket, &scopeMsg);

        AudioCrackMessageFirstCrack audioMsg;
        m_audioCrackClient.sendEvent(m_webSocket, &audioMsg);
    });

    m_serialport.addFunctionToMainCommand("IN,", [&](const char* comand) { 
        char* cpycommand = (char*)comand;
        char *lista = strtok(cpycommand, ",");

		lista = strtok(NULL, ",");
		m_applicationdata.tempET = ESPadapter::str2int(lista); 

		lista = strtok(NULL, ",");
		m_applicationdata.tempBT = ESPadapter::str2int(lista);

		lista = strtok(NULL, ",");
		m_applicationdata.porcentQuem = ESPadapter::str2int(lista);

		lista = strtok(NULL, ",");
		m_applicationdata.porcentTamb = ESPadapter::str2int(lista);

		lista = strtok(NULL, ",");
		m_applicationdata.porcentSopl = ESPadapter::str2int(lista);

		lista = strtok(NULL, ",");
		m_applicationdata.RoR = ESPadapter::str2int(lista);

		lista = strtok(NULL, ",");
		m_applicationdata.deltaETBT = ESPadapter::str2int(lista);

        AudioCrackMessageOperatives msg;
        msg.m_beanTemperature = m_applicationdata.tempBT;
        msg.m_rateOfRise = m_applicationdata.RoR;
        m_audioCrackClient.sendEvent(m_webSocket, &msg);
    });

    // This will be requested by the ATM
    m_serialport.addFunctionToMainCommand("PAIR", [&](const char* comand){

        m_clientHandler.unregisterWebsocketClient(m_visualScopeClient);
        m_clientHandler.unregisterWebsocketClient(m_audioCrackClient);
        ESPadapter::debug_println("WBS clients: " + String(m_clientHandler.getClientCount()));
        m_peer.executePairing(m_eepromdata);
        registerWebSocketHandler();
        ESPadapter::debug_println("WBS clients: " + String(m_clientHandler.getClientCount()));
    });

    //------------- Debug purposes ----------------

    m_serialport.addFunctionToMainCommand("STOPWS", [&](const char* comand){
        ESPadapter::serial_println("Stopping WebSocket server");
        WebsocketManager::turnOffWebSocket(m_webSocket);
    });

    m_serialport.addFunctionToMainCommand("SHOW", [&](const char* comand){
        m_applicationdata.print();
        m_eepromdata.print();
    });

    m_serialport.addFunctionToMainCommand("LOG1", [&](const char* comand){
        ESPadapter::serial_println("Debug traces enabled");
        ESPadapter::trace_debug = true;
    });

    m_serialport.addFunctionToMainCommand("LOGNULL", [&](const char* comand){
        ESPadapter::serial_println("All traces disabled");
        ESPadapter::trace_debug = false;
    });

    m_serialport.addFunctionToMainCommand("MEMOFREE", [&](const char* comand){
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

    m_visualScopeClient.setName("visualscope");
    m_clientHandler.registerWebsocketClient(m_visualScopeClient);

    m_visualScopeClient.addFunctionToMainCommand("getData", [&](uint8_t num, JsonDocument& doc) {

        //! No funciona, why ?
        // VisualScopeMessageOperatives scopeMsg;
        // scopeMsg.m_id = static_cast<int8_t>(doc["id"]);
        // scopeMsg.m_tempET = m_applicationdata.tempET;
        // scopeMsg.m_tempBT = m_applicationdata.tempBT;
        // scopeMsg.m_ror = m_applicationdata.RoR;
        // scopeMsg.m_porcentQuem = m_applicationdata.porcentQuem;
        // scopeMsg.m_porcentSopl = m_applicationdata.porcentSopl;
        // scopeMsg.m_porcentTamb = m_applicationdata.porcentTamb;
        // scopeMsg.m_deltaETBT = m_applicationdata.deltaETBT;
        // m_visualScopeClient.sendEvent(m_webSocket, &scopeMsg);

        std::string output;
        JsonDocument outdoc;
        outdoc["id"] = doc["id"];
        outdoc["data"]["aire"] = m_applicationdata.tempET;
        outdoc["data"]["grano"] = m_applicationdata.tempBT;
        outdoc["data"]["ror"] = m_applicationdata.RoR;
        outdoc["data"]["quemador"] = m_applicationdata.porcentQuem;
        outdoc["data"]["soplador"] = m_applicationdata.porcentSopl;
        outdoc["data"]["tambor"] = m_applicationdata.porcentTamb;
        outdoc["data"]["delta"] = m_applicationdata.deltaETBT;
        serializeJson(outdoc, output);
        m_webSocket.sendTXT(num, output);

        AudioCrackMessageOperatives audioMsg;
        audioMsg.m_beanTemperature = m_applicationdata.tempBT;
        audioMsg.m_rateOfRise = m_applicationdata.RoR;
        m_audioCrackClient.sendEvent(m_webSocket, &audioMsg);

        m_beat.set_status(Heartbeat::online);
    });

    m_visualScopeClient.addFunctionToMainCommand("setControlParams", [&](uint8_t num, JsonDocument& doc) {

        if (doc["params"].containsKey("aire"))
            m_applicationdata.aire = doc["params"]["aire"].as<int16_t>();

        if (doc["params"].containsKey("tambor"))    
            m_applicationdata.tambor = doc["params"]["tambor"].as<int16_t>();

        if (doc["params"].containsKey("quemador"))
            m_applicationdata.quemador = doc["params"]["quemador"].as<int16_t>();

        if (doc["params"].containsKey("tedvalue"))
            m_applicationdata.tedvalue = doc["params"]["tedvalue"].as<int16_t>();

        ESPadapter::serial_print("PARAM,");
        ESPadapter::serial_print(m_applicationdata.aire);ESPadapter::serial_print(',');
        ESPadapter::serial_print(m_applicationdata.tambor);ESPadapter::serial_print(',');
        ESPadapter::serial_print(m_applicationdata.quemador);ESPadapter::serial_print(',');
        ESPadapter::serial_print(m_applicationdata.tedvalue);ESPadapter::serial_print(',');
        ESPadapter::serial_write('\0');
        ESPadapter::retardo(50);
    });

    m_visualScopeClient.addFunctionToMainCommand("endRoasting", [&](uint8_t num, JsonDocument& doc) {
        
        AudioCrackMessageEndRoasting dropMessage;
        dropMessage.send(m_webSocket, num);
        
        ESPadapter::serial_print("SODROP");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
        m_beat.set_step(Heartbeat::other);
    });

    m_visualScopeClient.addFunctionToMainCommand("ready", [&](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("SREADY");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
        m_heartbeatonce = false;
    });

    m_visualScopeClient.addFunctionToMainCommand("noready", [&](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("SNOREA");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
        m_beat.set_step(Heartbeat::other);
        m_heartbeatonce = false;
    });

    m_visualScopeClient.addFunctionToMainCommand("identify", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("IDENTIFY");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
    });

    m_visualScopeClient.addFunctionToMainCommand("noidentify", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("NOIDENTIFY");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
    });

    m_visualScopeClient.addFunctionToMainCommand("getinit", [&](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("GETINIT");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
    });

    m_visualScopeClient.addFunctionToMainCommand("reset", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("RESET");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
    });

    m_visualScopeClient.addFunctionToMainCommand("fcstart", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("FCSTART");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
    });

    m_visualScopeClient.addFunctionToMainCommand("oncharge", [&](uint8_t num, JsonDocument& doc) {
        
        AudioCrackMessageStartRoasting startMessage;
        startMessage.send(m_webSocket, num);
        
        ESPadapter::serial_print("ONCHARGE");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
        m_beat.set_step(Heartbeat::roasting);
    });

    m_visualScopeClient.addFunctionToMainCommand("onted", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("ONTED");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
    });

    m_visualScopeClient.addFunctionToMainCommand("offted", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("OFFTED");
        ESPadapter::serial_print('\0');
        ESPadapter::retardo(50);
    });
}

void Manager::registerAudioCrack()
{
    ESPadapter::debug_println("Registering AudioCrack client...");
    
    m_audioCrackClient.setName("audiocrack");
    m_clientHandler.registerWebsocketClient(m_audioCrackClient);

    m_audioCrackClient.addFunctionToMainCommand("getinitial", [&](uint8_t num, JsonDocument& doc) {
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
        m_webSocket.sendTXT(num, output);
        ESPadapter::debug_print("TO-AUDIOCRACK: ");
        ESPadapter::debug_println(output);
    });

    m_audioCrackClient.addFunctionToMainCommand("firstcrack", [&](uint8_t num, JsonDocument& doc) {
        // Audiocrack device notification for first crack event
    });
}
