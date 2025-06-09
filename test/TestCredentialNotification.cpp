#include "Compiletype.hpp"
#ifdef TEST

#include <boost/test/unit_test.hpp>
#include "adapters/WebSocketAdapter.hpp"
#include "messages/ArtisanMessage.hpp"
#include "clients/AudioCrackClient.hpp"
#include "websocket/WebsocketClientHandler.hpp"
#include "websocket/CredentialNotification.hpp"
#include "CrossSectionalData.hpp"

BOOST_AUTO_TEST_CASE(given_NEW_CREDENTIALS_when_REGISTER_CLIENTS_ON_HANDLER_then_SEND_NOTIFICATION)
{
    CrossSectionalDataEEPROM originaldata;
    strcpy(originaldata.ssidSocket, "ROASTER");
    strcpy(originaldata.passSocket, "Clave1234");
    originaldata.canalwifi = 1;

    //------------

    const char* JSONPAYLOAD_PART_1 = R"({"deviceID":-1,"command":"newcredentials","parameters":{"ssid":"SSID FIJA","pass":"Clave fija","channel":5}})";
    const char* JSONPAYLOAD_PART_2 = R"({"deviceID":0,"command":"newcredentials","parameters":{"ssid":"SSID FIJA","pass":"Clave fija","channel":5}})";
    char JSONPAYLOAD_CONCAT[200] = {};
    strcat(JSONPAYLOAD_CONCAT, JSONPAYLOAD_PART_1);
    strcat(JSONPAYLOAD_CONCAT, JSONPAYLOAD_PART_2);

    WebSocketsServer websocket(8080);
    WebsocketClientHandler clienthandler(websocket);
    CrossSectionalDataEEPROM newdata;
    strcpy(newdata.ssidSocket, "SSID FIJA");
    strcpy(newdata.passSocket, "Clave fija");
    newdata.canalwifi = 5;

    ArtisanMessage artisanMsg;
    ArtisanClient artisan(&artisanMsg);

    AudioCrackClient audiocrack;

    clienthandler.registerWebsocketClient(artisan);
    clienthandler.registerWebsocketClient(audiocrack);

    websocket.cleanOutputString();

    CredentialNotification::notifyOnChange(websocket, clienthandler, newdata, originaldata);
    
    BOOST_CHECK_EQUAL(websocket.getOutputString().c_str(), JSONPAYLOAD_CONCAT);
}

BOOST_AUTO_TEST_CASE(given_SAME_CREDENTIALS_when_REGISTER_CLIENTS_ON_HANDLER_then_SEND_NOTIFICATION)
{
    CrossSectionalDataEEPROM originaldata;
    strcpy(originaldata.ssidSocket, "SSID FIJA");
    strcpy(originaldata.passSocket, "Clave fija");
    originaldata.canalwifi = 1;

    //------------

    const char* EXPECTED = R"()";

    WebSocketsServer websocket(8080);
    WebsocketClientHandler clienthandler(websocket);
    CrossSectionalDataEEPROM newdata;
    strcpy(newdata.ssidSocket, "SSID FIJA");
    strcpy(newdata.passSocket, "Clave fija");
    newdata.canalwifi = 1;

    ArtisanMessage artisanMsg;
    ArtisanClient artisan(&artisanMsg);

    AudioCrackClient audiocrack;

    clienthandler.registerWebsocketClient(artisan);
    clienthandler.registerWebsocketClient(audiocrack);

    websocket.cleanOutputString();

    CredentialNotification::notifyOnChange(websocket, clienthandler, newdata, originaldata);

    BOOST_CHECK_EQUAL(websocket.getOutputString().c_str(), EXPECTED);
}

#endif