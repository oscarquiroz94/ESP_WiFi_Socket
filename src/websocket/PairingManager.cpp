#include "PairingManager.hpp"
#include "utilities/Temporizador.hpp"
#include <algorithm>

void PairingManager::setupInitialPairing
(WebSocketsServer& webSocket, CrossSectionalDataEEPROM& data)
{
    // Apagar el websocket y el WiFi
    WebsocketManager::destroyWebSocket(webSocket);

    // Conectarse a la red wifi con datos de fabrica
    CrossSectionalDataEEPROM temporaryData;
    strcpy(temporaryData.ssidSocket,"ROASTER");
    strcpy(temporaryData.passSocket,"Clave123*");
    temporaryData.canalwifi = 1;
    if (!WebsocketManager::buildWebSocket(webSocket, temporaryData)) 
        return;

    // Crear cliente generico y meterlo al websocket para que escuche.
    // Con los datos recibidos se obtienen los id de los clientes externos
    // Y se guardan en la lista de clienthandler
    GenericMessage genericMessage;
    GenericClient genericClient(&genericMessage);
    WebsocketClientHandler clientHandler(webSocket);
    clientHandler.registerWebsocketClient(genericClient);

    genericClient.addFunctionToMainCommand("attach", [&](uint8_t num, WrapperJson::JsonDocument& doc) {
        //int8_t id = doc["deviceId"];
        std::string name = doc["deviceName"];
        
        if (std::find(data.clientNames.begin(), data.clientNames.end(), name) == data.clientNames.end()) 
        {
            data.clientNames.push_back(name);
            ESPadapter::serial_print("Client registered: ");
            ESPadapter::serial_println(name.c_str());
        }
    });

    webSocket.onEvent([&](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
        clientHandler.onWebSocketEvent(num, type, payload, length);
    });

    Temporizador t_search(false, true);
    uint32_t maxTimeSearch = 60L * 1000L; //seconds
    ESPadapter::serial_println("buscando...");
    while (!t_search.tiempo(maxTimeSearch))
        webSocket.loop();
    ESPadapter::serial_println("fin busqueda");

    // Enviar datos de la red wifi a los clientes registrados
    // La proxima vez que se reinicien todos, se conectaran a la red wifi definida por el usuario
    // Enviar/recibir datos normales
}