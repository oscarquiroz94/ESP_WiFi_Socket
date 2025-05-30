#pragma once

#include "Compiletype.hpp"
#include "websocket/PairingManager.hpp"
#include "CrossSectionalData.hpp"
#include "websocket/WebSocketsServerAdapter.hpp"

class PairingManagerAccess
{
    public:
        PairingManagerAccess(WebSocketsServer &ws) : peer(ws) {}

        void registerGenericClient
            (WebSocketsServer& webSocket, 
             CrossSectionalDataEEPROM& data)
        {
            peer.registerGenericClient(webSocket, data);
        }

        WebsocketClientHandler& getClientHandler()
        {
            return peer.getClientHandler();
        }

        ~PairingManagerAccess() = default;
    private:
        PairingManager peer;
};
