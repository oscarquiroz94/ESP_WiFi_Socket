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
        outdoc["id"] = doc["roasterID"];
        outdoc["data"]["aire"] = applicationdata.tempET;
        outdoc["data"]["grano"] = applicationdata.tempBT;
        outdoc["data"]["ror"] = applicationdata.RoR;
        outdoc["data"]["quemador"] = applicationdata.porcentQuem;
        outdoc["data"]["soplador"] = applicationdata.porcentSopl;
        outdoc["data"]["tambor"] = applicationdata.porcentTamb;
        outdoc["data"]["delta"] = applicationdata.deltaETBT;
        serializeJson(outdoc, output);
        webSocket.sendTXT(num, output);
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