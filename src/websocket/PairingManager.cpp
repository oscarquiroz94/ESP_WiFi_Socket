#include "PairingManager.hpp"
#include "utilities/Temporizador.hpp"
#include <algorithm>

void PairingManager::executePairing
    (CrossSectionalDataEEPROM& data)
{
    if (!setupDefaultCredentials(webSocket)) return;

    registerGenericClient(webSocket, data);

    searchingLoopForClients(webSocket);
    
    setupUserCredentials(webSocket, data);
}

bool PairingManager::setupDefaultCredentials(WebSocketsServer& webSocket)
{
    WebsocketManager::destroyWebSocket(webSocket);

    // Conectarse a la red wifi con datos de fabrica
    CrossSectionalDataEEPROM temporaryData;
    strcpy(temporaryData.ssidSocket,"ROASTER");
    strcpy(temporaryData.passSocket,"Clave123*");
    temporaryData.canalwifi = 1;

    return WebsocketManager::buildWebSocket(webSocket, temporaryData);
}

// TODO: testear esta funcion
void PairingManager::registerGenericClient
    (WebSocketsServer& webSocket, 
     CrossSectionalDataEEPROM& data)
{
    clientHandler.registerWebsocketClient(genericClient);

    genericClient.addFunctionToMainCommand("attach", [&](uint8_t num, JsonDocument& doc) {
        //int8_t id = doc["deviceId"];
        std::string name = doc["deviceName"];
        
        if (std::find(data.clientNames.begin(), data.clientNames.end(), name) == data.clientNames.end()) 
        {
            data.clientNames.push_back(name);
            ESPadapter::serial_print("Client registered: ");
            ESPadapter::serial_println(name.c_str());
        }
    });
}

// This function can not be tested due websocket.loop() has not native implementation
void PairingManager::searchingLoopForClients (WebSocketsServer& webSocket)
{
    Temporizador t_search(false, true);
    uint32_t maxTimeSearch = 60L * 1000L; //seconds

    webSocket.onEvent([&](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
        clientHandler.onWebSocketEvent(num, type, payload, length);
    });

    ESPadapter::serial_println("buscando...");
    while (!t_search.tiempo(maxTimeSearch))
        webSocket.loop();

    ESPadapter::serial_println("fin busqueda");
}

void PairingManager::setupUserCredentials
    (WebSocketsServer& webSocket, 
     CrossSectionalDataEEPROM& data)
{
    WebsocketManager::destroyWebSocket(webSocket);

    // Conectarse a la red wifi con datos definidos por usuario 
    WebsocketManager::buildWebSocket(webSocket, data);
}