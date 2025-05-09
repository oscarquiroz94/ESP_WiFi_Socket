#pragma once

#include "Compiletype.hpp"
#include "CrossSectionalData.hpp"
#include "WebSocketsServerAdapter.hpp"

extern WiFiClass WiFi;

class WebsocketManager
{
    public:
        static bool buildWebSocket(WebSocketsServer& webSocket, CrossSectionalData& data)
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
                ESPadapter::serial_println("Websocket running");

                String ssid = "SSID: " + String(data.ssidSocket);
                String ip = "IP: " + WiFi.softAPIP().toString();
                String channel = "Channel: " + String(WiFi.channel());
                ESPadapter::serial_println(ssid);
                ESPadapter::serial_println(ip);
                ESPadapter::serial_println(channel);
            }
            return success && webSocket.isRunning();
        }
};