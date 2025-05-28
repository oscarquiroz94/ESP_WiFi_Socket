// #include "AudioCrackClient.hpp"

// void AudioCrackClient::processEvent(uint8_t num, const char* payload, size_t length)
// {
//     // JsonDocument doc;
//     // DeserializationError err = deserializeJson(doc, payload);
//     // switch (err.code()) {
//     //     case DeserializationError::Ok:
//     //         break;
//     //     case DeserializationError::InvalidInput:
//     //         ESPadapter::serial_println("Invalid input!");
//     //         break;
//     //     case DeserializationError::NoMemory:
//     //         ESPadapter::serial_println("Not enough memory");
//     //         break;
//     //     default:
//     //         ESPadapter::serial_println("Deserialization failed");
//     //         break;
//     // }
    
//     // if (doc["clientID"].is<int8_t>()) 
//     //     this->id = doc["clientID"];
//     // if (!doc["clientID"].is<int8_t>() && this->id == -1) 
//     //     return; //! No es un mensaje conocido

//     // ESPadapter::serial_print("FROM-AUDIOCRACK: "); 
//     // ESPadapter::serial_println(payload);
// }