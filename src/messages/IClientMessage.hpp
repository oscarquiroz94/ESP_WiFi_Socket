#pragma once

#include "Compiletype.hpp"
#include "adapters/ESPadapter.hpp"
#include "ArduinoJson.hpp"

class IClientMessage
{
    public:
        virtual ~IClientMessage() = default;

        virtual int8_t getClientId() const = 0;

        /**
         * @brief Get the Document object
         * 
         * @param doc JsonDocument to be filled
         * @param data Payload to be parsed
         * @return true if the payload is a valid document for the instance
         * @return false if the payload is not a valid document for the instance
         */
        virtual bool getDocument(JsonDocument &doc, const char *data) = 0;

        /**
         * @brief Get the Command object
         * 
         * @param doc JsonDocument already filled
         * @return std::string main command of the document
         */
        virtual std::string getMainCommand(JsonDocument &doc) = 0;

    protected:
        int8_t id = -1;

        
        /** 
         * @brief Ask if the document is valid for the message type
         * @param doc 
         * @return true in case is valid, false otherwise
         */
        virtual bool isValid(JsonDocument &doc) = 0;

        
        /**
         * @brief Parse the payload into a Json document and handler errors
         * @param doc Input and Output Json document
         * @param payload Input stream payload
         */  
        void parsePayload(JsonDocument &doc, const char *payload)
        {
            DeserializationError err = deserializeJson(doc, payload);
            
            switch (err.code()) {
                case DeserializationError::Ok:
                    ESPadapter::debug_println("IClientmessage: Valid message!");
                    break;
                case DeserializationError::InvalidInput:
                    ESPadapter::debug_println("IClientmessage: Invalid message!");
                    break;
                case DeserializationError::NoMemory:
                    ESPadapter::debug_println("IClientmessage: Not enough memory");
                    break;
                default:
                    ESPadapter::debug_println("IClientmessage: Deserialization failed");
                    break;
            }
        }
};