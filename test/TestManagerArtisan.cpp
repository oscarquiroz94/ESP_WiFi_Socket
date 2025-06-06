#include "Compiletype.hpp"
#ifdef TEST

#include "ManagerAccess.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(given_GET_DATA_when_REGISTER_ARTISAN_HANDLER_then_EXPECTED)
{
    const char* JSONPAYLOAD_1 = R"({"command":"getData","id":2522,"roasterID":0})";
    const char* JSONPAYLOAD_2 = R"({"data":{"aire":100,"grano":150,"ror":5,"quemador":10,"soplador":20,"tambor":30,"delta":40}})";

    ManagerAccess manager;

    manager.registerWebSocketHandler();
    manager.getApplicationData().tempET = 100;
    manager.getApplicationData().tempBT = 150;
    manager.getApplicationData().RoR = 5;
    manager.getApplicationData().porcentQuem = 10;
    manager.getApplicationData().porcentSopl = 20;
    manager.getApplicationData().porcentTamb = 30;
    manager.getApplicationData().deltaETBT = 40; 

    manager.getClientHandler()
        .onWebSocketEvent(1, WStype_TEXT, (uint8_t*)JSONPAYLOAD_1, strlen(JSONPAYLOAD_1));

    BOOST_CHECK_EQUAL(manager.getWebsocket().getOutputString().c_str(), JSONPAYLOAD_2);
}

#endif