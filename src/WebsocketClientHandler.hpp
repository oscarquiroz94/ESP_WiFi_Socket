#pragma once

#include "ESPadapter.hpp"
#include "WebSocketsServerAdapter.hpp"
#include "IGeneralClient.hpp"
#include "ArtisanClient.hpp"

class WebsocketClientHandler
{
    public:
        WebsocketClientHandler(WebSocketsServer& ws) : webSocket(ws) {}

        void registerWebsocketClient(IGeneralClient& client);

        void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);

    private:
        WebSocketsServer& webSocket;
        std::vector<IGeneralClient*> websocketClients;   
};
