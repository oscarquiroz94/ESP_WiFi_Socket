#include "AudioCrackMessage.hpp"

// Messages received or send from audiocrack device
// {
//    "device":"audiocrack",
//    "command":"startroasting"
// }
// {
//    "device":"audiocrack",
//    "command":"firstcrack"
// }
// {
//    "device":"audiocrack",
//    "command":"endroasting"
// }
// {
//    "device":"audiocrack",
//    "command":"getinitial"
// }


bool AudioCrackMessage::getDocument(JsonDocument& doc, const char* data)
{
    parsePayload(doc, data);
    return isValid(doc);
}

std::string AudioCrackMessage::getMainCommand(JsonDocument& doc)
{
    if (!isValid(doc)) return "";

    if (doc["command"].is<std::string>()) 
        return doc["command"].as<std::string>();
    else return "";
}

bool AudioCrackMessage::isValid(JsonDocument& doc)
{
    return doc["device"].is<std::string>() && doc["command"].is<std::string>();
}

void AudioCrackMessageStartRoasting::send(WebSocketsServer& ws, int8_t id)
{
    std::string output;
    JsonDocument outdoc;

    outdoc["command"] = "startroasting";
    serializeJson(outdoc, output);
    ws.sendTXT(id, output);

    ESPadapter::debug_print("Sent audiocrack message: ");
    ESPadapter::debug_print(output.c_str());
    ESPadapter::debug_print(" id ");
    ESPadapter::debug_println(id);
}

void AudioCrackMessageEndRoasting::send(WebSocketsServer& ws, int8_t id)
{
    std::string output;
    JsonDocument outdoc;

    outdoc["command"] = "endroasting";
    serializeJson(outdoc, output);
    ws.sendTXT(id, output);

    ESPadapter::debug_print("Sent audiocrack message: ");
    ESPadapter::debug_print(output.c_str());
    ESPadapter::debug_print(" id ");
    ESPadapter::debug_println(id);
}

void AudioCrackMessageFirstCrack::send(WebSocketsServer& ws, int8_t id)
{
    std::string output;
    JsonDocument outdoc;

    outdoc["command"] = "firstcrack";
    serializeJson(outdoc, output);
    ws.sendTXT(id, output);

    ESPadapter::debug_print("Sent audiocrack message: ");
    ESPadapter::debug_print(output.c_str());
    ESPadapter::debug_print(" id ");
    ESPadapter::debug_println(id);
}

void AudioCrackMessageOperatives::send(WebSocketsServer& ws, int8_t id)
{
    std::string output;
    JsonDocument outdoc;

    outdoc["command"] = "operatives";
    outdoc["data"]["beantemperature"] = m_beanTemperature;
    outdoc["data"]["rateofrise"] = m_rateOfRise;
    serializeJson(outdoc, output);
    ws.sendTXT(id, output);

    ESPadapter::debug_print("Sent audiocrack message: ");
    ESPadapter::debug_print(output.c_str());
    ESPadapter::debug_print(" id ");
    ESPadapter::debug_println(id);
}