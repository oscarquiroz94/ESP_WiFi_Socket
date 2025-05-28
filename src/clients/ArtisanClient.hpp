#pragma once

#include "utilities/ESPadapter.hpp"
#include "IGeneralClient.hpp"
#include "messages/IClientMessage.hpp"
#include <functional>
#include <map>


class ArtisanClient : public IGeneralClient
{
    public:
        ArtisanClient(IClientMessage* msg) : message(msg) {}

        int8_t getClientId () const override {return message->getClientId();}

        std::string getName() const final {return "artisan";}

        void processEvent
            (uint8_t num, const char *payload, size_t length) override;

        void addFunctionToMainCommand
            (std::string key, 
             std::function<void(uint8_t num, JsonDocument& doc)> func) override;

        virtual ~ArtisanClient() = default;

    private:
        IClientMessage* message;
        std::map<std::string, std::function<void(uint8_t num, JsonDocument& doc)>> map2func;
};

