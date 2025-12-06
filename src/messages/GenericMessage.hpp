#include "IClientMessage.hpp"

class GenericMessage : public IClientMessage
{
    public:
        GenericMessage() = default;

        int8_t getId() const override {return id;}
        
        bool getDocument(JsonDocument& doc, const char* data) override
        {
            parsePayload(doc, data);
            return isValid(doc);
        }
        std::string getMainCommand(JsonDocument& doc) override
        {
            if (!isValid(doc)) return "";

            if (doc["command"].is<std::string>()) 
                return doc["command"].as<std::string>();
            else return "";
        }
    
    private:
        bool isValid(JsonDocument& doc) override
        {
            return doc["command"].is<std::string>() &&
                   doc["device"].is<std::string>();
        }
};