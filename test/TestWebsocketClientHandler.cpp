#include "Compiletype.hpp"
#ifdef TEST

#include "messages/VisualScopeMessage.hpp"
#include "clients/VisualScopeClient.hpp"
#include "clients/AudioCrackClient.hpp"
#include "websocket/WebsocketClientHandler.hpp"
#include "adapters/WebSocketAdapter.hpp"
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(given_JSONPAYLOAD_1_when_EVENTWB_then_WSTYPE_TEXT)
{
    const char* JSONPAYLOAD_1 = R"({"roasterID": 1, "command": "getData"})";

    WebSocketsServer webSocket = WebSocketsServer(8080);
    WebsocketClientHandler clientHandler(webSocket);

    VisualScopeMessage visualScopeMsg;
    VisualScopeClient visualScopeClient(&visualScopeMsg);
    AudioCrackClient audioCrackClient;

    clientHandler.registerWebsocketClient(visualScopeClient);
    clientHandler.registerWebsocketClient(audioCrackClient);

    bool visualScopeCallbackCalled = true;
    visualScopeClient.addFunctionToMainCommand("getData", [&](uint8_t num, JsonDocument& doc) {
        visualScopeCallbackCalled = true;
    });

    clientHandler.onWebSocketEvent(1, WStype_TEXT, (uint8_t*)JSONPAYLOAD_1, strlen(JSONPAYLOAD_1));

    BOOST_CHECK(true == visualScopeCallbackCalled);

}

#endif