#include "WebsocketClientHandler.hpp"

void WebsocketClientHandler::registerWebsocketClient
    (IGeneralClient& client)
{
    client.setId(m_idCounter++);
    websocketClients.push_back(&client);

    ESPadapter::debug_print("Client registered: ");
    ESPadapter::debug_print(client.getName().c_str());
    ESPadapter::debug_print(" id: ");
    ESPadapter::debug_println(client.getId());
}

void WebsocketClientHandler::unregisterWebsocketClient
    (IGeneralClient& client)
{
    auto it = std::find(websocketClients.begin(), websocketClients.end(), &client);
    if (it != websocketClients.end())
    {
        ESPadapter::debug_print("Client unregistered: ");
        ESPadapter::debug_print(client.getName().c_str());
        ESPadapter::debug_print(" id: ");
        ESPadapter::debug_println(client.getId());
        m_idCounter--;
        websocketClients.erase(it);
    }
    else
    {
        ESPadapter::debug_print("Client not found for unregistration: ");
        ESPadapter::debug_print(client.getName().c_str());
        ESPadapter::debug_print(" id: ");
        ESPadapter::debug_println(client.getId());
    }
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
            //No way to know which client disconnected
            ESPadapter::debug_println("Disconnected");
            break;
        }  

        // New client has connected
        case WStype_CONNECTED:
        {
            ESPadapter::debug_print("New client connected: ");
            IPAddress ip = webSocket.remoteIP(num);
            ESPadapter::debug_print(ip.toString());
            ESPadapter::debug_print(" id: ");
            ESPadapter::debug_println(num);
            break;
        }

        // Echo text message back to client
        case WStype_TEXT:
        {
            if (websocketClients.empty()) 
            {
                ESPadapter::debug_println("No clients registered to handle messages.");
                return;
            }
            
            for (auto& client : websocketClients)
            {
                if (nullptr != client)
                    client->processEvent(num, payloadconst, length);
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