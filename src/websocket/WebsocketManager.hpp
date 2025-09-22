#pragma once

#include "Compiletype.hpp"
#include "CrossSectionalData.hpp"
#include "adapters/WebSocketAdapter.hpp"

extern WiFiClass WiFi;

class WebsocketManager
{
    public:
        static bool turnOnWebSocket(WebSocketsServer& webSocket, const CrossSectionalDataEEPROM& data);
        static void turnOffWebSocket(WebSocketsServer& webSocket);
};
