#include "AudioCrackMessage.hpp"

// Messages received from audiocrack device
// {
//    "device":"audiocrack",
//    "command":"firstcrack"
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
    return (doc["device"].is<std::string>() && doc["device"] == "audiocrack") ? true : false;
}

void AudioCrackMessageStartRoasting::send(WebSocketsServer& ws, int8_t id)
{
    std::string output;
    JsonDocument outdoc;

    outdoc["command"] = "startroasting";
    serializeJson(outdoc, output);
    ws.sendTXT(id, output);
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
}