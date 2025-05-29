#include "ArtisanClient.hpp"
#include "utilities/WrapperJson.hpp"
#include "messages/ArtisanMessage.hpp"

void ArtisanClient::processEvent(uint8_t num, const char *payload, size_t length)
{ 
    WrapperJson::JsonDocument doc;

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
    std::function<void(uint8_t num, WrapperJson::JsonDocument& doc)> func)
{
    map2func[key] = func;
}