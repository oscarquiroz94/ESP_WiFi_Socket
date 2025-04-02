#pragma once

#include "compiletype.hpp"

#include "PuertoSerial.hpp"
#include "WebsocketManager.hpp"
#include "CrossSectionalData.hpp"

class Manager
{
    public:

        Manager() : puertoSerial(115200), webSocket(8080) {}

        void initialize()
        {
            puertoSerial.openPort();

            puertoSerial.addFunctionToCommand("S,", [&](const char* comand) { 
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
        void run()
        {
            webSocket.loop();

            puertoSerial.listen();
            puertoSerial.processEvent();
        }
    private:
        PuertoSerial puertoSerial;
        WebSocketsServer webSocket;
        CrossSectionalData applicationdata;
};