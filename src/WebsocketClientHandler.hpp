#pragma once

#include <Arduino.h>
#include <iostream>
#include <WebSocketsServer.h>
#include "GeneralClient.hpp"
#include "ArtisanClient.hpp"

class WebsocketClientHandler
{
    public:
        WebsocketClientHandler(WebSocketsServer& ws) : webSocket(ws) {}

        void registerWebsocketClient(GeneralClient& client)
        {
            websocketClients.push_back(&client);
            Serial.print("Client registered: ");
            Serial.println(client.getName().c_str());
        }

        void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
        {
            const char* payloadconst = (const char*)payload;

            switch (type)
            {
                // Client has disconnected
                case WStype_DISCONNECTED:
                {
                    //No way to know which client disconnected
                    Serial.println("Disconnected");
                    break;
                }  

                // New client has connected
                case WStype_CONNECTED:
                {
                    Serial.print("New client connected: ");
                    IPAddress ip = webSocket.remoteIP(num);
                    Serial.println(ip.toString());
                    break;
                }

                // Echo text message back to client
                case WStype_TEXT:
                {
                    if (websocketClients.empty()) return;
                    
                    for (auto& client : websocketClients)
                    {
                        client->processEvent(num, payloadconst, length);
                    }
                    
                    break;
                }

                case WStype_BIN:
                case WStype_ERROR:
                case WStype_FRAGMENT_TEXT_START:
                case WStype_FRAGMENT_BIN_START:
                case WStype_FRAGMENT:
                case WStype_FRAGMENT_FIN:
                default:
                    break;
            }
        }

    private:
        WebSocketsServer& webSocket;
        std::vector<GeneralClient*> websocketClients;   
};

// WebSocketsServer& WebsocketClientHandler::webSocket;
// std::vector<GeneralClient*> WebsocketClientHandler::websocketClients;