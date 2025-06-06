#pragma once

#include "adapters/ESPadapter.hpp"
#include "IOutputMessage.hpp"

class CredentialMessage : public IOutputMessage
{
    public: 
        void send(WebSocketsServer& ws, int8_t id)
        {
            ESPadapter::serial_println("send credential message");
        }
};

