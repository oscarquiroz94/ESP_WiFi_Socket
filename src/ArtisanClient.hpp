#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "GeneralClient.hpp"
#include <map>

class ArtisanClient : public GeneralClient
{
    public:

        std::string getName () const {return "artisan";}

        void processEvent(uint8_t num, const char *payload, size_t length) override
        { 
            Serial.print("request processed in artisan: ");Serial.println(payload);

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

            int16_t idMaquina = -1;
            if (doc["roasterID"].is<int16_t>()) 
                idMaquina = doc["roasterID"];
            if (!doc["roasterID"].is<int16_t>() && idMaquina == -1) 
                return; //! No es un mensaje de Artisan

            Serial.print("Artisan, ID Maquina: "); Serial.println(idMaquina);

            std::string comando = "";
            if (doc["command"].is<std::string>()) comando = doc["command"].as<std::string>();
            
            for (auto it = map2func.begin(); it != map2func.end(); it++)
            {
                if (comando == it->first)
                {
                    it->second(num);
                    break;
                }
            }
        }

        void addFunctionToCommand(std::string key, std::function<void(uint8_t num)> func)
        {
            map2func[key] = func;
        }

    private:
        std::map<std::string, std::function<void(uint8_t num)>> map2func;
};

