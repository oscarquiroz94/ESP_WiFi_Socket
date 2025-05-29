#pragma once

#include "Compiletype.hpp"
#include "utilities/ESPadapter.hpp"
#include "IClientMessage.hpp"

class ArtisanMessage : public IClientMessage
{
    public:        

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
            bool isvalid = false;
            if (doc["roasterID"].is<int8_t>()) 
            {
                this->id = doc["roasterID"];
                isvalid = true;
                // ESPadapter::serial_print("FROM-ARTISAN: ");
                // ESPadapter::serial_println(payload);
            }
            else if (!doc["roasterID"].is<int8_t>() && this->id == -1) 
                isvalid = false;

            return isvalid;
        }
};
