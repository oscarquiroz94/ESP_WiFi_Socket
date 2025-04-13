#pragma once

#include "ESPadapter.hpp"

#include <ArduinoJson.h>
#include "GeneralClient.hpp"
#include <functional>
#include <map>

class ArtisanClient : public GeneralClient
{
    public:

        std::string getName () const {return "artisan";}

        void processEvent(uint8_t num, const char *payload, size_t length) override
        { 
            ESPadapter::serial_print("request processed in artisan: ");
            ESPadapter::serial_println(payload);

            JsonDocument doc;
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

            int16_t machineId = -1;
            if (doc["roasterID"].is<int16_t>()) 
                machineId = doc["roasterID"];
            if (!doc["roasterID"].is<int16_t>() && machineId == -1) 
                return; //! No es un mensaje de Artisan

            ESPadapter::serial_print("Artisan, ID Maquina: ");
            ESPadapter::serial_println(machineId);

            //------------------

            std::string comando = "";
            if (doc["command"].is<std::string>()) comando = doc["command"].as<std::string>();
            
            for (auto it = map2func.begin(); it != map2func.end(); it++)
            {
                if (comando == it->first)
                {
                    it->second(num, doc);
                    break;
                }
            }
        }

        void addFunctionToCommand
        (   std::string key, 
            std::function<void(uint8_t num, JsonDocument& doc)> func)
        {
            map2func[key] = func;
        }

    private:
        std::map<std::string, std::function<void(uint8_t num, JsonDocument& doc)>> map2func;
};

