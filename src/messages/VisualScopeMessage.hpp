#pragma once

#include "Compiletype.hpp"
#include "adapters/ESPadapter.hpp"
#include "adapters/WebSocketAdapter.hpp"
#include "IClientMessage.hpp"
#include "IOutputMessage.hpp"

class VisualScopeMessage : public IClientMessage
{
    public:        

        int8_t getId() const override {return id;}

        bool getDocument(JsonDocument& doc, const char* data) override;

        std::string getMainCommand(JsonDocument& doc) override;
        
    private:
        bool isValid(JsonDocument& doc) override;
};

class VisualScopeMessageStartRoasting : public IOutputMessage
{
    public: void send(WebSocketsServer& ws, int8_t id) override;
};

class VisualScopeMessageEndRoasting : public IOutputMessage
{
    public: void send(WebSocketsServer& ws, int8_t id) override;
};

class VisualScopeMessageFirstCrack : public IOutputMessage
{
    public: void send(WebSocketsServer& ws, int8_t id) override;
};

class VisualScopeMessageOperatives : public IOutputMessage
{
    public:
        void send(WebSocketsServer& ws, int8_t id) override;

        int8_t m_id;
        int16_t m_tempET;
        int16_t m_tempBT;
        int16_t m_ror;
        int16_t m_porcentQuem;
        int16_t m_porcentSopl;
        int16_t m_porcentTamb;
        int16_t m_deltaETBT;
};
