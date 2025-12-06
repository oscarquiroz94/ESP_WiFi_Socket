#pragma once

#include "adapters/ESPadapter.hpp"
#include "IOutputMessage.hpp"

class CredentialMessage : public IOutputMessage
{
    public: 
        CredentialMessage(const CrossSectionalDataEEPROM &data)
            : m_data(data) {}

        void send(WebSocketsServer& ws, int8_t id) override
        {
            JsonDocument outdoc;
            std::string output;
            outdoc["command"] = "newcredentials";
            outdoc["parameters"]["ssid"] = m_data.ssidSocket;
            outdoc["parameters"]["pass"] = m_data.passSocket;
            outdoc["parameters"]["channel"] = m_data.canalwifi;

            serializeJson(outdoc, output);

            ESPadapter::debug_print("Credential message to client: ");
            ESPadapter::debug_print(id);
            ESPadapter::debug_print(" output: ");
            ESPadapter::debug_println(output.c_str());

            ws.sendTXT(id, output);
        }
    
        private:
            const CrossSectionalDataEEPROM& m_data;

};

