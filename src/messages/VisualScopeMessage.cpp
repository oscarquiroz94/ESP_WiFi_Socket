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

    ESPadapter::debug_print("Sent visualscope message: ");
    ESPadapter::debug_print(output.c_str());
    ESPadapter::debug_print(" id ");
    ESPadapter::debug_println(id);

    ws.sendTXT(id, output);
}

void VisualScopeMessageEndRoasting::send(WebSocketsServer& ws, int8_t id)
{
    std::string output;
    JsonDocument outdoc;

    outdoc["pushMessage"] = "endRoasting";
    serializeJson(outdoc, output);

    ESPadapter::debug_print("Sent visualscope message: ");
    ESPadapter::debug_print(output.c_str());
    ESPadapter::debug_print(" id ");
    ESPadapter::debug_println(id);

    ws.sendTXT(id, output);
}

void VisualScopeMessageFirstCrack::send(WebSocketsServer& ws, int8_t id)
{
    std::string output;
    JsonDocument outdoc;

    outdoc["pushMessage"] = "addEvent";
    outdoc["data"]["event"] = "firstCrackBeginningEvent";
    serializeJson(outdoc, output);

    ESPadapter::debug_print("Sent visualscope message: ");
    ESPadapter::debug_print(output.c_str());
    ESPadapter::debug_print(" id ");
    ESPadapter::debug_println(id);

    ws.sendTXT(id, output);
}

void VisualScopeMessageOperatives::send(WebSocketsServer& ws, int8_t id)
{
    std::string output;
    JsonDocument outdoc;
    
    outdoc["id"] = static_cast<uint8_t>(m_id);
    outdoc["data"]["aire"] = m_tempET;
    outdoc["data"]["grano"] = m_tempBT;
    outdoc["data"]["ror"] = m_ror;
    outdoc["data"]["quemador"] = m_porcentQuem;
    outdoc["data"]["soplador"] = m_porcentSopl;
    outdoc["data"]["tambor"] = m_porcentTamb;
    outdoc["data"]["delta"] = m_deltaETBT;
    serializeJson(outdoc, output);

    ESPadapter::debug_print("Sent visualscope message: ");
    ESPadapter::debug_print(output.c_str());
    ESPadapter::debug_print(" id ");
    ESPadapter::debug_println(id);

    ws.sendTXT(id, output);
}