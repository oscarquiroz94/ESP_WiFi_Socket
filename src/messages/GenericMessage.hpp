#include "IClientMessage.hpp"

class GenericMessage : public IClientMessage
{
    public:
        GenericMessage() = default;

        int8_t getClientId() const override {return id;}
        
        bool getDocument(WrapperJson::JsonDocument& doc, const char* data) override
        {
            parsePayload(doc, data);
            return isValid(doc);
        }
        std::string getMainCommand(WrapperJson::JsonDocument& doc) override
        {
            if (!isValid(doc)) return "";

            if (doc["command"].is<std::string>()) 
                return doc["command"].as<std::string>();
            else return "";
        }
    
    private:
        bool isValid(WrapperJson::JsonDocument& doc) override
        {
            return doc["deviceID"].is<int8_t>();
        }
};