#pragma once

#include "ESPadapter.hpp"

class GeneralClient
{
    public:

        GeneralClient() = default;

        virtual void processEvent(uint8_t num, const char* payload, size_t length) = 0;
        virtual int8_t getId () const = 0;
        virtual void setId(int8_t val) = 0;
        virtual std::string getName() const = 0;

        virtual ~GeneralClient() = default;

    protected:
        int8_t id = -1;
};
