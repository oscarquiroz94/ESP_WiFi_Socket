#pragma once

#include "CrossSectionalData.hpp"
#include "adapters/WebSocketAdapter.hpp"
#include "messages/CredentialMessage.hpp"

class CredentialNotification
{
    public:
        CredentialNotification() = default;

        static void notifyOnChange
        (   WebSocketsServer &ws,
            WebsocketClientHandler &clienthandler,
            CrossSectionalDataEEPROM &data)
        {
            CredentialMessage msg;
            
            clienthandler.doForeachClient([&ws, &msg](IGeneralClient* client) {
                // A todos los clientes enviar el mismo mensaje
                notify(client, ws, &msg);
            });
        }

    private:
        static void notify(IGeneralClient* client, WebSocketsServer& ws, IOutputMessage *msg)
        {
            client->sendEvent(ws, msg);
        }
};

