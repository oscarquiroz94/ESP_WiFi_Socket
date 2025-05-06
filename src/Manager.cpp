#include "Manager.hpp"
#include "ESPadapter.hpp"

void Manager::initialize()
{
    serialport.openPort();

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
}

void Manager::run()
{
    webSocket.loop();

    serialport.listen();
    serialport.processEvent();
}