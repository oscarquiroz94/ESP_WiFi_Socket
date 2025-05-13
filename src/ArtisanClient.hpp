#pragma once

#include "ESPadapter.hpp"
#include "GeneralClient.hpp"
#include "IClientMessage.hpp"
#include <functional>
#include <map>


class ArtisanClient : public GeneralClient
{
    public:
        ArtisanClient(IClientMessage* msg) : message(msg) {}

        int8_t getClientId () const override {return message->getClientId();}

        std::string getName() const final {return "artisan";}

        void processEvent
            (uint8_t num, const char *payload, size_t length) override;

        void addFunctionToCommand
            (std::string key, 
             std::function<void(uint8_t num, JsonDocument& doc)> func);

        virtual ~ArtisanClient() = default;

    private:
        IClientMessage* message;
        std::map<std::string, std::function<void(uint8_t num, JsonDocument& doc)>> map2func;
};

