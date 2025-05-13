#pragma once

#include "Compiletype.hpp"
#include "ESPadapter.hpp"

class IClientMessage
{
    public:
        virtual ~IClientMessage() = default;

        virtual int8_t getClientId() const = 0;
        virtual bool getDocument(JsonDocument& doc, const char* data) = 0;
        virtual std::string getCommand(JsonDocument& doc) = 0;
    protected:
        int8_t id = -1;
};