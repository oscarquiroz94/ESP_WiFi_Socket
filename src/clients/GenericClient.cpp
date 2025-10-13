#include "GenericClient.hpp"

void GenericClient::processEvent(uint8_t num, const char* payload, size_t length)
{
    JsonDocument doc;

    if (nullptr == message) {ESPadapter::print_null(CLASSNAME, __func__); return;}

    if (!message->getDocument(doc, payload)) return;

    for (auto it = map2func.begin(); it != map2func.end(); it++)
    {
        if (message->getMainCommand(doc) == it->first)
        {
            it->second(num, doc);
            break;
        }
    }
}

void GenericClient::addFunctionToMainCommand
    (std::string key, 
        std::function<void(uint8_t num, JsonDocument& doc)> func)
{
    map2func[key] = func;
}

void GenericClient::setId(uint8_t num, const char* payload)
{
    JsonDocument doc;
    if (!message->getDocument(doc, payload)) return;
    if (doc["device"] == name) 
        m_id = num;

    // Special case for Artisan
    if (doc["roasterID"].is<int8_t>()) 
        m_id = num;

    // Special case for pairing
    if (doc["command"] == "attach")
        m_id = num;

    ESPadapter::debug_print("Client ");
    ESPadapter::debug_print(name.c_str());
    ESPadapter::debug_print(" assigned id: ");
    ESPadapter::debug_println(m_id);
}

void GenericClient::sendEvent(WebSocketsServer &ws, IOutputMessage* msg)
{
    if (m_id == -1) 
    {
        ESPadapter::debug_print("Client ");
        ESPadapter::debug_print(name.c_str());
        ESPadapter::debug_println(" has no id assigned, cannot send message");
        return;
    }
    msg->send(ws, m_id);
}