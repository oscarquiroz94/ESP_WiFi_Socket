#pragma once

#include <ArduinoJson.h>
#include "GeneralClient.hpp"

class AudioCrackClient : public GeneralClient
{
    public:

        std::string getName () const {return "audiocrack";}

        void processEvent(uint8_t num, const char* payload, size_t length) override
        {
            Serial.print("request processed in audiocrack: ");Serial.println(payload);

            JsonDocument doc;
            DeserializationError err = deserializeJson(doc, payload);
            switch (err.code()) {
                case DeserializationError::Ok:
                    break;
                case DeserializationError::InvalidInput:
                    Serial.println(F("Invalid input!"));
                    break;
                case DeserializationError::NoMemory:
                    Serial.println(F("Not enough memory"));
                    break;
                default:
                    Serial.println(F("Deserialization failed"));
                    break;
            }
            
            int16_t clientId = -1;
            if (doc["clientID"].is<int16_t>()) 
                clientId = doc["clientID"];
            if (!doc["clientID"].is<int16_t>() && clientId == -1) 
                return; //! No es un mensaje conocido

            Serial.print("AudioCrack, ID Maquina: "); Serial.println(clientId);
        }
};