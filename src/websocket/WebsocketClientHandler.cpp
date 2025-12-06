#include "WebsocketClientHandler.hpp"
#include <algorithm>

void WebsocketClientHandler::registerWebsocketClient
    (IGeneralClient& client)
{
    m_idCounter++;
    m_websocketClients.push_back(&client);

    ESPadapter::debug_print("Client registered: ");
    ESPadapter::debug_println(client.getName().c_str());
}

void WebsocketClientHandler::unregisterWebsocketClient
    (IGeneralClient& client)
{
    auto it = std::find(m_websocketClients.begin(), m_websocketClients.end(), &client);
    if (it != m_websocketClients.end())
    {
        ESPadapter::debug_print("Client unregistered: ");
        ESPadapter::debug_print(client.getName().c_str());
        ESPadapter::debug_print(" id: ");
        ESPadapter::debug_println(client.getId());
        m_idCounter--;
        m_websocketClients.erase(it);
    }
    else
    {
        ESPadapter::debug_print("Client not found for unregistration: ");
        ESPadapter::debug_print(client.getName().c_str());
        ESPadapter::debug_print(" id: ");
        ESPadapter::debug_println(client.getId());
    }
}

void WebsocketClientHandler::doForeachClient(std::function<void(IGeneralClient*)> func) 
{ 
    for (auto &client : m_websocketClients)
    {
        if (nullptr != client)
            func(client);
    }
}

std::string WebsocketClientHandler::getClientNames()
{
    std::string names = "";
    for (auto& client : m_websocketClients)
    {
        if (nullptr != client)
            names += client->getName() + " - ";
    }
    return names;
}

void WebsocketClientHandler::onWebSocketEvent
    (uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    const char* payloadconst = (const char*)payload;

    switch (type)
    {
        // Client has disconnected
        case WStype_DISCONNECTED:
        {
            // Eliminar id del cliente
            for (auto& client : m_websocketClients)
            {
                if (nullptr != client)
                    client->unsetId(num);
            }
            
            break;
        }  

        // New client has connected
        case WStype_CONNECTED:
        {
            ESPadapter::debug_print("New client connected: ");
            IPAddress ip = m_webSocket.remoteIP(num);
            ESPadapter::debug_print(ip.toString());
            ESPadapter::debug_print(" id: ");
            ESPadapter::debug_println(num);
            break;
        }

        // Echo text message back to client
        case WStype_TEXT:
        {
            ESPadapter::debug_print("Message from client id ");
            ESPadapter::debug_print((uint16_t)num);
            ESPadapter::debug_print(": ");
            ESPadapter::debug_println(payloadconst);

            if (m_websocketClients.empty()) 
            {
                ESPadapter::debug_println("No clients registered to handle messages.");
                return;
            }

            // Registrar id del cliente segun el device del mensaje
            for (auto& client : m_websocketClients)
            {
                if (nullptr != client)
                    client->setId(num, payloadconst);
            }

            // Procesar evento solo para el cliente correspondiente
            for (auto& client : m_websocketClients)
            {
                if (nullptr != client && client->getId() == num)
                {
                    ESPadapter::debug_print("Processing event for client id: ");
                    ESPadapter::debug_println(num);
                    client->processEvent(num, payloadconst, length);
                    break;
                }
            }
            
            break;
        }

        case WStype_BIN:
        case WStype_ERROR:
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
        default:
            break;
    }
}