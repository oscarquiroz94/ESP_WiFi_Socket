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
        PairingManager(WebSocketsServer& ws, WebsocketClientHandler& handler);

        void executePairing
            (CrossSectionalDataEEPROM& data);

        void setMaxTimeSearch(uint32_t time) 
        {
            maxTimeSearch = time;
        }

        ~PairingManager() = default;
             
    private:
        WebSocketsServer& webSocket;
        GenericMessage    genericMessage;
        GenericClient     genericClient;
        WebsocketClientHandler& clientHandler;

        uint32_t maxTimeSearch = 1 * 60L * 1000L; // 60 seconds

        void registerGenericClient(CrossSectionalDataEEPROM& data);

        void searchingLoopForClients();

        bool isExistingPairingNetwork();

        bool setupDefaultCredentials();

        void setupUserCredentials(CrossSectionalDataEEPROM& data);

        WebsocketClientHandler& getClientHandler() {return clientHandler;}

        friend class PairingManagerAccess;
};