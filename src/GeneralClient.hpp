#pragma once

#include "ESPadapter.hpp"

class GeneralClient
{
    public:

        GeneralClient() = default;

        virtual void processEvent(uint8_t num, const char* payload, size_t length) = 0;
        virtual std::string getName () const = 0;

        virtual ~GeneralClient() = default;
};
