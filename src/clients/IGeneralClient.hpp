#pragma once

#include "adapters/ESPadapter.hpp"
#include <functional>

class IGeneralClient
{
    public:

        IGeneralClient() = default;

        virtual void processEvent(uint8_t num, const char* payload, size_t length) = 0;

        virtual void addFunctionToMainCommand
            (std::string key, 
             std::function<void(uint8_t num, JsonDocument& doc)> func) = 0;

        virtual int8_t getClientId () const = 0;
        virtual std::string getName() const = 0;

        virtual ~IGeneralClient() = default;
};
