#pragma once

#include "adapters/ESPadapter.hpp"
#include "IOutputMessage.hpp"

class CredentialMessage : public IOutputMessage
{
    public: 
        CredentialMessage(const CrossSectionalDataEEPROM &data)
            : _data(data) {}

        void send(WebSocketsServer& ws, int8_t id)
        {
            std::string output;
            JsonDocument outdoc;

            outdoc["deviceID"] = id;
            outdoc["command"] = "newcredentials";
            outdoc["parameters"]["ssid"] = _data.ssidSocket;
            outdoc["parameters"]["pass"] = _data.passSocket;
            outdoc["parameters"]["channel"] = _data.canalwifi;
            serializeJson(outdoc, output);
            ws.sendTXT(id, output);
        }
    
        private:
            const CrossSectionalDataEEPROM& _data;

};

