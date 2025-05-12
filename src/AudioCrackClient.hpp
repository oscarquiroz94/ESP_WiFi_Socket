#pragma once

#include <ArduinoJson.h>
#include "ESPadapter.hpp"
#include "GeneralClient.hpp"

class AudioCrackClient : public GeneralClient
{
    public:

        int8_t getId () const override {return id;}
        void setId(int8_t val) override {id = val;}

        std::string getName() const final {return "audiocrack";}

        void processEvent
            (uint8_t num, const char* payload, size_t length) override;
};