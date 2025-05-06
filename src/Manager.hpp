#pragma once

#include "compiletype.hpp"

#include "SerialPort.hpp"
#include "WebsocketManager.hpp"
#include "CrossSectionalData.hpp"

class Manager
{
    public:

        Manager() : serialport(115200), webSocket(8080) {}

        void initialize();

        void run();

    private:
        SerialPort serialport;
        WebSocketsServer webSocket;
        CrossSectionalData applicationdata;
};