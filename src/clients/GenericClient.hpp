#pragma once

#include <functional>
#include <map>
#include "IGeneralClient.hpp"
#include "messages/IClientMessage.hpp"

#define CLASSNAME "GenericClient"


class GenericClient : public IGeneralClient
{
    public:
        GenericClient(IClientMessage* msg) : message(msg) {}

        void processEvent(uint8_t num, const char* payload, size_t length) override;

        void addFunctionToMainCommand
            (std::string key, 
             std::function<void(uint8_t num, JsonDocument& doc)> func) override;
        
        void setId(uint8_t num, const char* payload) override;
        int8_t getId() const override {return m_id;}

        void unsetId(uint8_t num) override;

        void setName(const std::string& newname) override {name = newname;}
        std::string getName() const override {return name;}

        void sendEvent(WebSocketsServer &ws, IOutputMessage* msg) override;

        ~GenericClient() = default;

    private:
        int8_t m_id {-1};
        std::string name {""};
        IClientMessage* message = nullptr;
        std::map<std::string, std::function<void(uint8_t num, JsonDocument& doc)>> map2func;
};