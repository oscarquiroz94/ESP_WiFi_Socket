#pragma once

#include "Compiletype.hpp"
#include "ESPadapter.hpp"
#include "IClientMessage.hpp"

class ArtisanMessage : public IClientMessage
{
    public:        

        int8_t getClientId() const override {return id;}

        bool getDocument(JsonDocument& doc, const char* data) override
        {
            parsePayload(doc, data);
            return isArtisanMsg;
        }

        std::string getCommand(JsonDocument& doc) override
        {
            if (!isArtisanMsg) return "";

            if (doc["command"].is<std::string>()) 
                return doc["command"].as<std::string>();
            else return "";
        }


    private:
        bool isArtisanMsg = false;

        void parsePayload(JsonDocument& doc, const char* payload)
        {
            DeserializationError err = deserializeJson(doc, payload);
            
            switch (err.code()) {
                case DeserializationError::Ok:
                    break;
                case DeserializationError::InvalidInput:
                    ESPadapter::serial_println("Invalid input!");
                    break;
                case DeserializationError::NoMemory:
                    ESPadapter::serial_println("Not enough memory");
                    break;
                default:
                    ESPadapter::serial_println("Deserialization failed");
                    break;
            }

            if (doc["roasterID"].is<int8_t>()) 
            {
                this->id = doc["roasterID"];
                this->isArtisanMsg = true;
                // ESPadapter::serial_print("FROM-ARTISAN: ");
                // ESPadapter::serial_println(payload);
            }
            else if (!doc["roasterID"].is<int8_t>() && this->id == -1) 
                this->isArtisanMsg = false; 
        }
};
