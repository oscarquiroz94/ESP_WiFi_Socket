#include "VisualScopeMessage.hpp"

bool VisualScopeMessage::getDocument(JsonDocument& doc, const char* data)
{
    parsePayload(doc, data);
    return isValid(doc);
}

std::string VisualScopeMessage::getMainCommand(JsonDocument& doc)
{
    if (!isValid(doc)) return "";

    if (doc["command"].is<std::string>()) 
        return doc["command"].as<std::string>();
    else return "";
}

bool VisualScopeMessage::isValid(JsonDocument& doc)
{
    bool isvalid = false;
    if (doc["roasterID"].is<int8_t>()) 
    {
        this->id = doc["roasterID"];
        isvalid = true;
    }
    else if (!doc["roasterID"].is<int8_t>() && this->id == -1) 
        isvalid = false;

    return isvalid;
}

void VisualScopeMessageStartRoasting::send(WebSocketsServer& ws, int8_t id)
{
    std::string output;
    JsonDocument outdoc;

    outdoc["pushMessage"] = "startRoasting";
    serializeJson(outdoc, output);
    ws.sendTXT(id, output);
}

void VisualScopeMessageEndRoasting::send(WebSocketsServer& ws, int8_t id)
{
    std::string output;
    JsonDocument outdoc;

    outdoc["pushMessage"] = "endRoasting";
    serializeJson(outdoc, output);
    ws.sendTXT(id, output);
}

void VisualScopeMessageFirstCrack::send(WebSocketsServer& ws, int8_t id)
{
    std::string output;
    JsonDocument outdoc;

    outdoc["pushMessage"] = "addEvent";
    outdoc["data"]["event"] = "firstCrackBeginningEvent";
    serializeJson(outdoc, output);
    ws.sendTXT(id, output);
}