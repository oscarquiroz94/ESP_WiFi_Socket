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
            {Manager::m_serialport.processEvent();}

        void setMaxTimeSearch(uint32_t time) 
            {Manager::m_peer.setMaxTimeSearch(time);}

        WebSocketsServer& getWebsocket() {return Manager::m_webSocket;}

        WebsocketClientHandler& getClientHandler() {return Manager::m_clientHandler;}
        CrossSectionalData& getApplicationData() {return Manager::m_applicationdata;}
};
