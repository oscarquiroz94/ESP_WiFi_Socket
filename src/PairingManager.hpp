#pragma once

#include "Compiletype.hpp"
#include "ESPadapter.hpp"
#include "CrossSectionalData.hpp"
#include "WebsocketManager.hpp"
#include "WebSocketsServer.h"
#include "GenericClient.hpp"
#include "GenericMessage.hpp"
#include "WebsocketClientHandler.hpp"

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
        PairingManager() = default;

        void setupInitialPairing
            (WebSocketsServer& webSocket,
             CrossSectionalDataEEPROM& data);
             

        void setupNewCredentials()
        {
            //Enviarle a los clientes registrados la nueva configuracion
        }

        ~PairingManager() = default;
};