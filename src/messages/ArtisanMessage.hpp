#pragma once

#include "Compiletype.hpp"
#include "adapters/ESPadapter.hpp"
#include "adapters/WebSocketAdapter.hpp"
#include "IClientMessage.hpp"
#include "IOutputMessage.hpp"

class ArtisanMessage : public IClientMessage
{
    public:        

        int8_t getClientId() const override {return id;}

        bool getDocument(JsonDocument& doc, const char* data) override;

        std::string getMainCommand(JsonDocument& doc) override;
        
    private:
        bool isValid(JsonDocument& doc) override;
};

class ArtisanMessageStartRoasting : public IOutputMessage
{
    public: void send(WebSocketsServer& ws, int8_t id);
};

class ArtisanMessageEndRoasting : public IOutputMessage
{
    public: void send(WebSocketsServer& ws, int8_t id);
};

class ArtisanMessageFirstCrack : public IOutputMessage
{
    public: void send(WebSocketsServer& ws, int8_t id);
};
