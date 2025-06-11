#pragma once

#include "compiletype.hpp"

#include "serial/SerialPort.hpp"
#include "websocket/WebsocketManager.hpp"
#include "websocket/WebsocketClientHandler.hpp"
#include "CrossSectionalData.hpp"
#include "clients/ArtisanClient.hpp"
#include "clients/AudioCrackClient.hpp"
#include "messages/ArtisanMessage.hpp"
#include "websocket/PairingManager.hpp"
#include "utilities/Heartbeat.hpp"

/*
* @brief Class Manager
* 
* This class is the monolithic process of the application. 
* It register the serial port and websocket handlers, and manages the
* communication between the different components of the application.
* Therefore, could have many changes in the future.
*/
class Manager
{
    public:

        Manager() : 
            serialport(115200) 
            ,webSocket(8080)
            ,clientHandler(webSocket)
            ,artisanClient(&artisanMsg)
            ,peer(webSocket) 
            {}

        void initialize();
        void run();

    private:
        CrossSectionalData applicationdata;
        CrossSectionalDataEEPROM eepromdata;
        SerialPort serialport;
        WebSocketsServer webSocket;

        ArtisanMessage artisanMsg;
        
        WebsocketClientHandler clientHandler;
        ArtisanClient artisanClient;
        AudioCrackClient audioCrackClient;

        PairingManager peer;
        Heartbeat beat;
    
        uint8_t  sendVersionAmount = 0;
        uint32_t t_sendversion = 0;
        uint32_t versionESP = 0;

        void registerSerialPortHandler();
        void registerWebSocketHandler();
        void registerArtisan();
        void registerAudioCrack();
        void send_data();

        friend class ManagerAccess;
};