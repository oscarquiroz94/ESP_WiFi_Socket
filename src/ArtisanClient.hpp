#pragma once

#include "ESPadapter.hpp"
#include <ArduinoJson.h>
#include "GeneralClient.hpp"
#include <functional>
#include <map>


class ArtisanClient : public GeneralClient
{
    public:
        ArtisanClient() = default;

        std::string getName () const override {return "artisan";}

        void processEvent
            (uint8_t num, const char *payload, size_t length) override;

        void addFunctionToCommand
            (std::string key, 
             std::function<void(uint8_t num, JsonDocument& doc)> func);

        virtual ~ArtisanClient() = default;

    private:
        std::map<std::string, std::function<void(uint8_t num, JsonDocument& doc)>> map2func;
};

