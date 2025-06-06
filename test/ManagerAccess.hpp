#pragma once

#include "Compiletype.hpp"
#include "Manager.hpp"
#include "adapters/WebSocketAdapter.hpp"
#include "websocket/WebsocketClientHandler.hpp"

class ManagerAccess : public Manager
{
    public:
        ManagerAccess() : Manager() {}

        void registerSerialPortHandler() 
            {Manager::registerSerialPortHandler();}

        void registerWebSocketHandler()
            {Manager::registerWebSocketHandler();}

        void processEvent() 
            {Manager::serialport.processEvent();}

        void setMaxTimeSearch(uint32_t time) 
            {Manager::peer.setMaxTimeSearch(time);}

        WebSocketsServer& getWebsocket() {return webSocket;}

        WebsocketClientHandler& getClientHandler() {return clientHandler;}

        CrossSectionalData& getApplicationData() {return applicationdata;}
};
