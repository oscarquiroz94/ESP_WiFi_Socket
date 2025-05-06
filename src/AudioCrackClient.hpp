#pragma once

#include <ArduinoJson.h>
#include "ESPadapter.hpp"
#include "GeneralClient.hpp"

class AudioCrackClient : public GeneralClient
{
    public:

        std::string getName () const {return "audiocrack";}

        void processEvent
            (uint8_t num, const char* payload, size_t length) override;
};