#include "ArtisanClient.hpp"

void ArtisanClient::processEvent(uint8_t num, const char *payload, size_t length)
{ 
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

    if (doc["roasterID"].is<int8_t>()) 
        this->id = doc["roasterID"];
    if (!doc["roasterID"].is<int8_t>() && this->id == -1) 
        return; //! No es un mensaje de Artisan

    // ESPadapter::serial_print("FROM-ARTISAN: ");
    // ESPadapter::serial_println(payload);

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

void ArtisanClient::addFunctionToCommand
(   std::string key, 
    std::function<void(uint8_t num, JsonDocument& doc)> func)
{
    map2func[key] = func;
}