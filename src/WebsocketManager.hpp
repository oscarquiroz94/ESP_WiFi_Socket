#pragma once

#include "Compiletype.hpp"
#include "CrossSectionalData.hpp"
#include "WebSocketsServerAdapter.hpp"

extern WiFiClass WiFi;

class WebsocketManager
{
    public:
        static bool buildWebSocket(WebSocketsServer& webSocket, CrossSectionalDataEEPROM& data);
        static void destroyWebSocket(WebSocketsServer& webSocket);
};
