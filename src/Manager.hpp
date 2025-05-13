#pragma once

#include "compiletype.hpp"

#include "SerialPort.hpp"
#include "WebsocketManager.hpp"
#include "WebsocketClientHandler.hpp"
#include "CrossSectionalData.hpp"
#include "ArtisanClient.hpp"
#include "AudioCrackClient.hpp"
#include "ArtisanMessage.hpp"

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
            serialport(115200), 
            webSocket(8080), 
            clientHandler(webSocket),
            artisanClient(&artisanMsg) {}

        void initialize();

        void run();

        void sendInitializationData(const uint32_t version);

    private:
        CrossSectionalData applicationdata;
        SerialPort serialport;
        WebSocketsServer webSocket;

        ArtisanMessage artisanMsg;
        
        WebsocketClientHandler clientHandler;
        ArtisanClient artisanClient;
        AudioCrackClient audioCrackClient;
    
        uint8_t  sendVersionAmount = 0;
        uint32_t t_sendversion = 0;

        void registerSerialPortHandler();
        void registerWebSocketHandler();
};