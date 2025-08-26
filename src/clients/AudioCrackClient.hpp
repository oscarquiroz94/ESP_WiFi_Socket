#pragma once

#include <functional>
#include <map>
#include "adapters/ESPadapter.hpp"
#include "adapters/WebSocketAdapter.hpp"
#include "messages/IClientMessage.hpp"
#include "messages/IOutputMessage.hpp"
#include "GenericClient.hpp"

//class AudioCrackClient : public GenericClient {}

// class AudioCrackClient : public IGeneralClient
// {
//     public:

//         int8_t getClientId () const override {return 0;}

//         std::string getName() const final {return "audiocrack";}

//         void processEvent
//             (uint8_t num, const char* payload, size_t length) override;

//         void addFunctionToMainCommand
//             (std::string key, 
//              std::function<void(uint8_t num, JsonDocument& doc)> func) override;

//         void sendEvent(WebSocketsServer &ws, IOutputMessage* msg) override;

//     private:
//         IClientMessage* message;
//         std::map<std::string, std::function<void(uint8_t num, JsonDocument& doc)>> map2func;
// };