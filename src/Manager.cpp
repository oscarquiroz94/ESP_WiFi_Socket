#include "Manager.hpp"
#include "ESPadapter.hpp"


void Manager::initialize()
{
    serialport.openPort();
    registerSerialPortHandler();
    registerWebSocketHandler();
}

void Manager::registerSerialPortHandler()
{
    serialport.addFunctionToCommand("S,", [&](const char* comand) { 
        //Limpiar valores anteriores
        memset(applicationdata.ssidSocket, 0, 50);
        memset(applicationdata.passSocket, 0, 50);

        char* cpycommand = (char*)comand;
        char *lista = strtok(cpycommand, ",");

        lista = strtok(NULL, ",");
        if (lista != NULL) strcpy(applicationdata.ssidSocket, String(lista).c_str());

        lista = strtok(NULL, ",");
        if (lista != NULL) strcpy(applicationdata.passSocket, String(lista).c_str());

        lista = strtok(NULL, ",");
        if (lista != NULL) applicationdata.canalwifi = (uint8_t)(String(lista).toInt());

        WebsocketManager::buildWebSocket(webSocket, applicationdata); 
    });

    // TODO: AGREGAR A TESTING
    serialport.addFunctionToCommand("MCA", [&](const char* comand){
        String output;
        JsonDocument outdoc;
        int8_t idArtisan = artisanClient.getId();

        outdoc["pushMessage"] = "startRoasting";
        serializeJson(outdoc, output);
        webSocket.sendTXT(idArtisan, output);
    });

    // TODO: AGREGAR A TESTING
    serialport.addFunctionToCommand("MDR", [&](const char* comand){
        String output;
        JsonDocument outdoc;
        int8_t idArtisan = artisanClient.getId();

        outdoc["pushMessage"] = "endRoasting";
        serializeJson(outdoc, output);
        webSocket.sendTXT(idArtisan, output);
    });

    // TODO: AGREGAR A TESTING
    serialport.addFunctionToCommand("MFC", [&](const char* comand){
        String output;
        JsonDocument outdoc;
        int8_t idArtisan = artisanClient.getId();

        outdoc["pushMessage"] = "addEvent";
		outdoc["data"]["event"] = "firstCrackBeginningEvent";
        serializeJson(outdoc, output);
        webSocket.sendTXT(idArtisan, output);
    });

    // TODO: AGREGAR A TESTING
    // IN,230,160,1500,2000,200,600,200 --> ET, BT, Q, T, S, ROR, delta
    serialport.addFunctionToCommand("IN,", [&](const char* comand) { 
        char* cpycommand = (char*)comand;
        char *lista = strtok(cpycommand, ",");

		lista = strtok(NULL, ",");
		if (lista != NULL) applicationdata.tempET = String(lista).toInt(); 

		lista = strtok(NULL, ",");
		if (lista != NULL) applicationdata.tempBT = String(lista).toInt();

		lista = strtok(NULL, ",");
		if (lista != NULL) applicationdata.porcentQuem = String(lista).toInt();

		lista = strtok(NULL, ",");
		if (lista != NULL) applicationdata.porcentTamb = String(lista).toInt();

		lista = strtok(NULL, ",");
		if (lista != NULL) applicationdata.porcentSopl = String(lista).toInt();

		lista = strtok(NULL, ",");
		if (lista != NULL) applicationdata.RoR = String(lista).toInt();

		lista = strtok(NULL, ",");
		if (lista != NULL) applicationdata.deltaETBT = String(lista).toInt();
    });
}

void Manager::registerWebSocketHandler()
{
    clientHandler.registerWebsocketClient(artisanClient);
    clientHandler.registerWebsocketClient(audioCrackClient);

    artisanClient.addFunctionToCommand("getData", [&](uint8_t num, JsonDocument& doc) {
        String output;
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
        // ESPadapter::serial_print("TO-ARTISAN: ");
        // ESPadapter::serial_println(output);
    });

    artisanClient.addFunctionToCommand("setControlParams", [&](uint8_t num, JsonDocument& doc) {
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

    artisanClient.addFunctionToCommand("endRoasting", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("SODROP");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToCommand("ready", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("SREADY");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToCommand("noready", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("SNOREA");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToCommand("identify", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("IDENTIFY");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToCommand("noidentify", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("NOIDENTIFY");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToCommand("getinit", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("GETINIT");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToCommand("reset", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("RESET");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToCommand("fcstart", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("FCSTART");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToCommand("oncharge", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("ONCHARGE");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToCommand("onted", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("ONTED");ESPadapter::serial_print('\0');
    });

    artisanClient.addFunctionToCommand("offted", [](uint8_t num, JsonDocument& doc) {
        ESPadapter::serial_print("OFFTED");ESPadapter::serial_print('\0');
    });

    webSocket.onEvent([&](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
        clientHandler.onWebSocketEvent(num, type, payload, length);
    });
}

void Manager::run()
{
    webSocket.loop();

    serialport.listen();
    serialport.processEvent();
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