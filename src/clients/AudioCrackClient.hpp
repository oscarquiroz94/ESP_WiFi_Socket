#pragma once

#include "utilities/ESPadapter.hpp"
#include "IGeneralClient.hpp"

class AudioCrackClient : public IGeneralClient
{
    public:

        int8_t getClientId () const override {return 0;}

        std::string getName() const final {return "audiocrack";}

        void processEvent
            (uint8_t num, const char* payload, size_t length) override
        {
            //ESPadapter::serial_println("Audiocrack: Not implemented yet");
        }

        void addFunctionToMainCommand
            (std::string key, 
             std::function<void(uint8_t num, JsonDocument& doc)> func) override
        {
            //ESPadapter::serial_println("Audiocrack: Not implemented yet");
        }
};