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

        void processEvent(uint8_t num, const char* payload, size_t length) override
        {
            JsonDocument doc;

            if (nullptr == message) {ESPadapter::print_null(CLASSNAME, __func__); return;}

            if (!message->getDocument(doc, payload)) return;

            for (auto it = map2func.begin(); it != map2func.end(); it++)
            {
                if (message->getMainCommand(doc) == it->first)
                {
                    it->second(num, doc);
                    break;
                }
            }
        }

        void addFunctionToMainCommand
            (std::string key, 
             std::function<void(uint8_t num, JsonDocument& doc)> func) override
        {
            map2func[key] = func;
        }

        uint8_t getId () const override {return m_id;}
        void setId(uint8_t id) override {m_id = id;}

        void setName(const std::string& newname) override {name = newname;}
        std::string getName() const override {return name;}

        void sendEvent(WebSocketsServer &ws, IOutputMessage* msg) override 
        {
            msg->send(ws, getId());
        }

        ~GenericClient() = default;

    private:
        uint8_t m_id {0};
        std::string name {"generic"};
        IClientMessage* message = nullptr;
        std::map<std::string, std::function<void(uint8_t num, JsonDocument& doc)>> map2func;
};