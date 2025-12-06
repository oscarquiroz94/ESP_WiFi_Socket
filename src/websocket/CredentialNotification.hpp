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
            const CrossSectionalDataEEPROM &newdata,
            CrossSectionalDataEEPROM &olddata)
        {
            if (olddata == newdata) return;
            
            CredentialMessage msg(newdata);

            ESPadapter::debug_println("Credentials changed, notifying clients...");
            ESPadapter::debug_print("WBS clients: ");
            ESPadapter::debug_println(clienthandler.getClientCount());
            ESPadapter::debug_print("WBS client names: ");
            ESPadapter::debug_println(clienthandler.getClientNames().c_str());
            
            clienthandler.doForeachClient([&ws, &msg](IGeneralClient* client) {
                ESPadapter::debug_print("Notifying client: ");
                ESPadapter::debug_print(client->getName().c_str());
                ESPadapter::debug_print(" id: ");
                ESPadapter::debug_println(client->getId());
                notify(client, ws, &msg);
            });
        }

        static void notify(IGeneralClient* client, WebSocketsServer& ws, IOutputMessage *msg)
        {
            client->sendEvent(ws, msg);
        }
};

