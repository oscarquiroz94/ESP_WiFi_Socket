#pragma once

#include "Compiletype.hpp"
#include "adapters/ESPadapter.hpp"
#include "adapters/WebSocketAdapter.hpp"
#include "IClientMessage.hpp"
#include "IOutputMessage.hpp"

class AudioCrackMessage : public IClientMessage
{
    public:        

        int8_t getId() const override {return id;}

        bool getDocument(JsonDocument& doc, const char* data) override;

        std::string getMainCommand(JsonDocument& doc) override;
        
    private:
        bool isValid(JsonDocument& doc) override;
};


class AudioCrackMessageStartRoasting : public IOutputMessage
{
    public: void send(WebSocketsServer& ws, int8_t id);
};

class AudioCrackMessageEndRoasting : public IOutputMessage
{
    public: void send(WebSocketsServer& ws, int8_t id);
};

class AudioCrackMessageFirstCrack : public IOutputMessage
{
    public: void send(WebSocketsServer& ws, int8_t id);
};

class AudioCrackMessageOperatives : public IOutputMessage
{
    public: 
        void send(WebSocketsServer& ws, int8_t id);
        int16_t m_beanTemperature;
        int16_t m_rateOfRise;
};
