#include "ArtisanClient.hpp"
#include "ArtisanMessage.hpp"

void ArtisanClient::processEvent(uint8_t num, const char *payload, size_t length)
{ 
    JsonDocument doc;

    // Si no es un documento de artisan, no se procesa
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

void ArtisanClient::addFunctionToMainCommand
(   std::string key, 
    std::function<void(uint8_t num, JsonDocument& doc)> func)
{
    map2func[key] = func;
}