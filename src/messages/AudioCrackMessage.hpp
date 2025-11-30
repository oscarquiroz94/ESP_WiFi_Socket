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
    public: void send(WebSocketsServer& ws, int8_t id) override;
};

class AudioCrackMessageEndRoasting : public IOutputMessage
{
    public: void send(WebSocketsServer& ws, int8_t id) override;
};

class AudioCrackMessageFirstCrack : public IOutputMessage
{
    public: void send(WebSocketsServer& ws, int8_t id) override;
};

class AudioCrackMessageOperatives : public IOutputMessage
{
    public: 
        void send(WebSocketsServer& ws, int8_t id) override;
        int16_t m_beanTemperature;
        int16_t m_rateOfRise;
};
