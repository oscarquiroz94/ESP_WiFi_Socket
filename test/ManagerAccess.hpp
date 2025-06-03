#pragma once

#include "Compiletype.hpp"
#include "Manager.hpp"

class ManagerAccess : public Manager
{
    public:
        ManagerAccess() : Manager() {}

        void registerSerialPortHandler() 
            {Manager::registerSerialPortHandler();}

        void processEvent() 
            {Manager::serialport.processEvent();}

        void setMaxTimeSearch(uint32_t time) 
            {Manager::peer.setMaxTimeSearch(time);}
};
