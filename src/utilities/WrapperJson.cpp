#include "WrapperJson.hpp"

namespace WrapperJson
{
    size_t serializeJson(const JsonDocument& doc, String &output) 
    {
        return serializeJson(doc, output);
    }
    DeserializationError deserializeJson(JsonDocument &doc, const char *payload) 
    {
        return deserializeJson(doc, payload);
    }
}