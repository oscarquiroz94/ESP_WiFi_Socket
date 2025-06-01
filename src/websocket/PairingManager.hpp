#pragma once

#include "Compiletype.hpp"
#include "adapters/ESPadapter.hpp"
#include "CrossSectionalData.hpp"
#include "websocket/WebsocketManager.hpp"
#include "adapters/WebSocketAdapter.hpp"
#include "clients/GenericClient.hpp"
#include "messages/GenericMessage.hpp"
#include "websocket/WebsocketClientHandler.hpp"

/*
* @brief Class PairingManager
* 
* This class handles all the monolithic process for pairing the clients.
* It changes connection, receive clients, register them and send data to them.
* Therefore, could have many changes in the future.
*/
class PairingManager
{ 
    public:
        PairingManager(WebSocketsServer& ws) : 
            webSocket(ws),
            genericClient(&genericMessage), 
            clientHandler(ws) {}

        void executePairing
            (CrossSectionalDataEEPROM& data);

        ~PairingManager() = default;
             
    private:
        WebSocketsServer& webSocket;
        GenericMessage genericMessage;
        GenericClient  genericClient;
        WebsocketClientHandler clientHandler;

        void registerGenericClient
            (WebSocketsServer& webSocket,
             CrossSectionalDataEEPROM& data);

        void searchingLoopForClients(WebSocketsServer& webSocket);

        bool setupDefaultCredentials(WebSocketsServer& webSocket);

        void setupUserCredentials
            (WebSocketsServer& webSocket,
             CrossSectionalDataEEPROM& data);

        WebsocketClientHandler& getClientHandler() {return clientHandler;}

        friend class PairingManagerAccess;
};