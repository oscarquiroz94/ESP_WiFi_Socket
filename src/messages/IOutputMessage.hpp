#pragma once

#include "adapters/ESPadapter.hpp"
#include "adapters/WebSocketAdapter.hpp"

class IOutputMessage
{
    public:
        virtual void send(WebSocketsServer& ws, int8_t id) = 0;
};