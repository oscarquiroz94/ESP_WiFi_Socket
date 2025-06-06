#pragma once

#include "adapters/ESPadapter.hpp"
#include "adapters/WebSocketAdapter.hpp"
#include "clients/IGeneralClient.hpp"
#include "clients/ArtisanClient.hpp"

class WebsocketClientHandler
{
    public:
        WebsocketClientHandler(WebSocketsServer& ws) : webSocket(ws) {}

        void registerWebsocketClient(IGeneralClient& client);

        void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);

        void doForeachClient(std::function<void(IGeneralClient*)> func) 
        { 
            for (auto &client : websocketClients)
            {
                func(client);
            }
        }

    private:
        WebSocketsServer& webSocket;
        std::vector<IGeneralClient*> websocketClients;   
};
