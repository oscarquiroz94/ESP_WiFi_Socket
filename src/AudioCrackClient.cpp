#include "AudioCrackClient.hpp"

void AudioCrackClient::processEvent(uint8_t num, const char* payload, size_t length)
{
    ESPadapter::serial_print("request processed in audiocrack: ");
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
    
    int16_t clientId = -1;
    if (doc["clientID"].is<int16_t>()) 
        clientId = doc["clientID"];
    if (!doc["clientID"].is<int16_t>() && clientId == -1) 
        return; //! No es un mensaje conocido

    ESPadapter::serial_print("AudioCrack, ID Maquina: "); ESPadapter::serial_println(clientId);
}