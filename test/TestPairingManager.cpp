#include "Compiletype.hpp"
#ifdef TEST

#include "websocket/PairingManager.hpp"
#include "PairingManagerAccess.hpp"
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(given_ATTACH_MSG_when_PAIRING_then_CLIENTS_ON_VECTOR)
{
    const char* JSONPAYLOAD_1 = R"({"deviceID":1,"command":"attach","deviceName":"TestClient"})";
    const char* JSONPAYLOAD_2 = R"({"deviceID":2,"command":"attach","deviceName":"TestClient2"})";

    const char* JSONPAYLOAD_3 = R"({"deviceID":1,"command":"newcredentials","parameters":{"ssid":"TEST_SSID","pass":"ClaveTest*","channel":5}})";
    const char* JSONPAYLOAD_4 = R"({"deviceID":2,"command":"newcredentials","parameters":{"ssid":"TEST_SSID","pass":"ClaveTest*","channel":5}})";

    WebSocketsServer webSocket(8080);
    CrossSectionalDataEEPROM eepromdata;
    PairingManagerAccess peer_access(webSocket);

    memset(eepromdata.ssidSocket, 0, sizeof(eepromdata.ssidSocket));
    memset(eepromdata.passSocket, 0, sizeof(eepromdata.passSocket));
    strcpy(eepromdata.ssidSocket, "TEST_SSID");
    strcpy(eepromdata.passSocket, "ClaveTest*");
    eepromdata.canalwifi = 5;

    peer_access.registerGenericClient(webSocket, eepromdata);

    
    // Simulate receiving the attach messages from two clients
    //
    webSocket.cleanOutputString();
    peer_access.getClientHandler()
                .onWebSocketEvent(1, WStype_TEXT, (uint8_t*)JSONPAYLOAD_1, strlen(JSONPAYLOAD_1));

    BOOST_CHECK_EQUAL(webSocket.getOutputString().c_str(), JSONPAYLOAD_3);

    webSocket.cleanOutputString();
    peer_access.getClientHandler()
                .onWebSocketEvent(1, WStype_TEXT, (uint8_t*)JSONPAYLOAD_2, strlen(JSONPAYLOAD_2));

    BOOST_CHECK_EQUAL(webSocket.getOutputString().c_str(), JSONPAYLOAD_4);
    //--------

    // Check if the clients were registered correctly
    BOOST_REQUIRE_EQUAL(eepromdata.clientNames.size(), 2);
    BOOST_CHECK_EQUAL(eepromdata.clientNames[0], "TestClient");
    BOOST_CHECK_EQUAL(eepromdata.clientNames[1], "TestClient2");

    
}

#endif