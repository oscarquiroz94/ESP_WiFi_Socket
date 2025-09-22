#pragma once

#include "Compiletype.hpp"
#include "websocket/PairingManager.hpp"
#include "CrossSectionalData.hpp"
#include "adapters/WebSocketAdapter.hpp"

class PairingManagerAccess
{
    public:
        PairingManagerAccess(WebSocketsServer &ws) : peer(ws) {}

        void registerGenericClient
            (CrossSectionalDataEEPROM& data)
        {
            peer.registerGenericClient(data);
        }

        WebsocketClientHandler& getClientHandler()
        {
            return peer.getClientHandler();
        }

        ~PairingManagerAccess() = default;
    private:
        PairingManager peer;
};
