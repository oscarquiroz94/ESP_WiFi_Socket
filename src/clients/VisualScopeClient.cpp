#include "VisualScopeClient.hpp"
#include "messages/VisualScopeMessage.hpp"

void VisualScopeClient::processEvent(uint8_t num, const char *payload, size_t length)
{ 
    JsonDocument doc;

    if (nullptr == message) {ESPadapter::print_null("VisualScopeClient", __func__); return;}

    // Si no es un documento de visualScope, no se procesa
    if (!message->getDocument(doc, payload)) return;
    
    for (auto it = map2func.begin(); it != map2func.end(); it++)
    {
        if (nullptr == it->second) continue;
        if (message->getMainCommand(doc) == it->first)
        {
            it->second(num, doc);
            break;
        }
    }
}

void VisualScopeClient::addFunctionToMainCommand
(   std::string key, 
    std::function<void(uint8_t num, JsonDocument& doc)> func)
{
    map2func[key] = func;
}

void VisualScopeClient::sendEvent(WebSocketsServer &ws, IOutputMessage* msg)
{
    msg->send(ws, getClientId());
}