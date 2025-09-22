#include "PairingManager.hpp"
#include "utilities/Temporizador.hpp"
#include <algorithm>

PairingManager::PairingManager(WebSocketsServer& ws, WebsocketClientHandler& handler) : 
            webSocket(ws),
            genericClient(&genericMessage), 
            clientHandler(handler) {}

void PairingManager::executePairing
    (CrossSectionalDataEEPROM& newdata)
{
    if (!setupDefaultCredentials()) return;

    registerGenericClient(newdata);

    searchingLoopForClients();

    setupUserCredentials(newdata);
}

bool PairingManager::setupDefaultCredentials()
{
    // Conectarse a la red wifi con datos de fabrica
    CrossSectionalDataEEPROM temporaryData;
    strcpy(temporaryData.ssidSocket,"PAIRING");
    strcpy(temporaryData.passSocket,"Clave123");
    temporaryData.canalwifi = 1;

    return WebsocketManager::turnOnWebSocket(webSocket, temporaryData);
}

void PairingManager::registerGenericClient
    (CrossSectionalDataEEPROM& data)
{
    clientHandler.registerWebsocketClient(genericClient);

    // {
    //     “deviceid” : m [uint8],
    //     “command”: “attach”,
    //     “devicename” :“audiocrack”,
    // }
    genericClient.addFunctionToMainCommand("attach", [&](uint8_t num, JsonDocument& doc) {
        int8_t id = doc["deviceid"];
        uint8_t idClient = genericClient.getId();

        std::string name = doc["devicename"];
        
        JsonDocument outdoc;
        std::string output;
        outdoc["deviceid"] = id;
        outdoc["command"] = "newcredentials";
        outdoc["parameters"]["ssid"] = data.ssidSocket;
        outdoc["parameters"]["pass"] = data.passSocket;
        outdoc["parameters"]["channel"] = data.canalwifi;

        if (name.empty() || name == "null") 
        {
            ESPadapter::debug_println("PairingManager: deviceName is empty");
            return;
        }
        
        if (std::find(data.clientNames.begin(), data.clientNames.end(), name) == data.clientNames.end()) 
        {
            data.clientNames.push_back(name);
            ESPadapter::debug_print("Peer client registered: ");
            ESPadapter::debug_println(name.c_str());

            serializeJson(outdoc, output);
            webSocket.sendTXT(idClient, output);
        }
    });
}

// This function can not be tested due websocket.loop() has not native implementation
void PairingManager::searchingLoopForClients()
{
    Temporizador t_search(false, true);

    webSocket.onEvent([&](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
        clientHandler.onWebSocketEvent(num, type, payload, length);
    });

    ESPadapter::debug_println("PairingManager: buscando...");
    while (!t_search.tiempo(maxTimeSearch))
    {
        webSocket.loop();
        ESPadapter::retardo(5);
    }
        
    clientHandler.unregisterWebsocketClient(genericClient);
    ESPadapter::debug_println("PairingManager: fin busqueda");

}

void PairingManager::setupUserCredentials
    (CrossSectionalDataEEPROM& data)
{
    // Conectarse a la red wifi con datos definidos por usuario 
    bool success = WebsocketManager::turnOnWebSocket(webSocket, data);
    if (!success) 
        ESPadapter::debug_println("PairingManager: Error starting WebSocket with user credentials");
}