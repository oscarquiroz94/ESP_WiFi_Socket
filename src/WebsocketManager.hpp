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

#ifdef DEPLOY
                String ssid = String("SSID: ") + String(data.ssidSocket);
                String ip = String("IP: ") + WiFi.softAPIP().toString();
                String channel = String("Channel: ") + String(WiFi.channel());
#else
                std::string ssid = std::string("SSID: ") + std::string(data.ssidSocket);
                std::string ip = std::string("IP: ") + std::string("192.168.4.1");
                std::string channel = std::string("Channel: ") + std::to_string(data.canalwifi);
#endif
                ESPadapter::serial_println(ssid);
                ESPadapter::serial_println(ip);
                ESPadapter::serial_println(channel);
            }
            return success && webSocket.isRunning();
        }
};