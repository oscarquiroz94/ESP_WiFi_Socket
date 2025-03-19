#pragma once

#include <Arduino.h>

class GeneralClient
{
    public:

        virtual void processEvent(uint8_t num, const char* payload, size_t length) = 0;
        virtual std::string getName () const = 0;
};
