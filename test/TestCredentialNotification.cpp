#include "Compiletype.hpp"
#ifdef TEST

#include <boost/test/unit_test.hpp>
#include "adapters/WebSocketAdapter.hpp"
#include "messages/ArtisanMessage.hpp"
#include "websocket/WebsocketClientHandler.hpp"
#include "websocket/CredentialNotification.hpp"
#include "CrossSectionalData.hpp"

BOOST_AUTO_TEST_CASE(given_NEW_CREDENTIALS_when_REGISTER_CLIENTS_ON_HANDLER_then_SEND_NOTIFICATION)
{
    WebSocketsServer websocket(8080);
    WebsocketClientHandler clienthandler(websocket);
    CrossSectionalDataEEPROM data;

    ArtisanMessage artisanMsg;
    ArtisanClient artisan(&artisanMsg);

    clienthandler.registerWebsocketClient(artisan);

    CredentialNotification::notifyOnChange(websocket, clienthandler, data);
}

#endif