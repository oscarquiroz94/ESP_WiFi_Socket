#pragma once

#include "ESPadapter.hpp"
#include "GeneralClient.hpp"

class AudioCrackClient : public GeneralClient
{
    public:

        int8_t getClientId () const override {return 0;}

        std::string getName() const final {return "audiocrack";}

        void processEvent
            (uint8_t num, const char* payload, size_t length) override;
};