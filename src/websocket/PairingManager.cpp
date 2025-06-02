#include "PairingManager.hpp"
#include "utilities/Temporizador.hpp"
#include <algorithm>

void PairingManager::executePairing
    (CrossSectionalDataEEPROM& data)
{
    if (!setupDefaultCredentials(webSocket)) return;

    registerGenericClient(webSocket, data);

    searchingLoopForClients(webSocket);
}

bool PairingManager::setupDefaultCredentials(WebSocketsServer& webSocket)
{
    // Conectarse a la red wifi con datos de fabrica
    CrossSectionalDataEEPROM temporaryData;
    strcpy(temporaryData.ssidSocket,"PAIRING");
    strcpy(temporaryData.passSocket,"Clave123*");
    temporaryData.canalwifi = 1;

    return WebsocketManager::buildWebSocket(webSocket, temporaryData);
}

void PairingManager::registerGenericClient
    (WebSocketsServer& webSocket, 
     CrossSectionalDataEEPROM& data)
{
    clientHandler.registerWebsocketClient(genericClient);

    genericClient.addFunctionToMainCommand("attach", [&](uint8_t num, JsonDocument& doc) {
        int8_t id = doc["deviceID"];
        int8_t idClient = genericClient.getClientId();

        std::string name = doc["deviceName"];
        
        JsonDocument outdoc;
        std::string output;
        outdoc["deviceID"] = id;
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
            ESPadapter::debug_print("Client registered: ");
            ESPadapter::debug_println(name.c_str());

            serializeJson(outdoc, output);
            webSocket.sendTXT(idClient, output);
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

    ESPadapter::debug_println("PairingManager: buscando...");
    while (!t_search.tiempo(maxTimeSearch))
    {
        webSocket.loop();
        ESPadapter::retardo(5);
    }
        
    ESPadapter::debug_println("PairingManager: fin busqueda");
}