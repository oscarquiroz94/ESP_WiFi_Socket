#include "Compiletype.hpp"
#ifdef TEST

#include "messages/ArtisanMessage.hpp"
#include "clients/ArtisanClient.hpp"
#include "clients/AudioCrackClient.hpp"
#include "websocket/WebsocketClientHandler.hpp"
#include "adapters/WebSocketAdapter.hpp"
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(given_JSONPAYLOAD_1_when_EVENTWB_then_WSTYPE_TEXT)
{
    const char* JSONPAYLOAD_1 = R"({"roasterID": 1, "command": "getData"})";

    WebSocketsServer webSocket = WebSocketsServer(8080);
    WebsocketClientHandler clientHandler(webSocket);

    ArtisanMessage artisanMsg;
    ArtisanClient artisanClient(&artisanMsg);
    AudioCrackClient audioCrackClient;

    clientHandler.registerWebsocketClient(artisanClient);
    clientHandler.registerWebsocketClient(audioCrackClient);

    bool artisanCallbackCalled = true;
    artisanClient.addFunctionToMainCommand("getData", [&](uint8_t num, JsonDocument& doc) {
        artisanCallbackCalled = true;
    });

    clientHandler.onWebSocketEvent(1, WStype_TEXT, (uint8_t*)JSONPAYLOAD_1, strlen(JSONPAYLOAD_1));

    BOOST_CHECK(true == artisanCallbackCalled);

}

#endif