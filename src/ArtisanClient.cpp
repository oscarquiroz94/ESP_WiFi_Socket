#include "ArtisanClient.hpp"
#include "ArtisanMessage.hpp"

void ArtisanClient::processEvent(uint8_t num, const char *payload, size_t length)
{ 
    JsonDocument doc;
    if (!message->getDocument(doc, payload)) return;
    
    for (auto it = map2func.begin(); it != map2func.end(); it++)
    {
        if (message->getCommand(doc) == it->first)
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