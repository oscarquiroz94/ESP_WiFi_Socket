#pragma once

#include <WebSocketsServer.h>
#include <WiFi.h>
#include "CrossSectionalData.hpp"

class WebsocketManager
{
    public:
        static void buildWebSocket(WebSocketsServer& webSocket, CrossSectionalData& data)
        {
            bool success = false;
            WiFi.mode(WIFI_OFF);
            WiFi.mode(WIFI_AP);
            WiFi.setSleep(false);
            //checkNetwork();

            success = WiFi.softAP(data.ssidSocket, data.passSocket, data.canalwifi);
            webSocket.begin();

            if (success && webSocket.isRunning())
            {
                Serial.println("Websocket running");
            }
        }
};

