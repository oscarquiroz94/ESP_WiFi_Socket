#include "Compiletype.hpp"
#ifdef TEST

#include "messages/VisualScopeMessage.hpp"
#include "clients/VisualScopeClient.hpp"
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(given_JSONPAYLOAD_1_when_EVENTWB_then_SETPARAMS)
{
    const char* JSONPAYLOAD_1 = R"({"roasterID": 1, "command": "setControlParams", "params": {"aire": 100, "tambor": 80, "quemador": 60, "soplador": 50}})";

    VisualScopeMessage visualScopeMsg;
    VisualScopeClient visualScopeClient(&visualScopeMsg);
    bool callbackCalled = false;

    visualScopeClient.addFunctionToMainCommand("setControlParams", [&](uint8_t num, JsonDocument& doc) {
        callbackCalled = true;
        BOOST_CHECK(1 == doc["roasterID"]);
        BOOST_CHECK(100 == doc["params"]["aire"]);
        BOOST_CHECK(80 == doc["params"]["tambor"]);
        BOOST_CHECK(60 == doc["params"]["quemador"]);
        BOOST_CHECK(50 == doc["params"]["soplador"]);
    });

    visualScopeClient.processEvent(128, JSONPAYLOAD_1, sizeof(JSONPAYLOAD_1));

    BOOST_CHECK(true == callbackCalled);
}


// Cases for single command are cover here 
BOOST_AUTO_TEST_CASE(given_JSONPAYLOAD_2_when_EVENTWB_then_GETDATA)
{
    const char* JSONPAYLOAD_2 = R"({"command":"getData","id":92295,"roasterID":0})";

    VisualScopeMessage visualScopeMsg;
    VisualScopeClient visualScopeClient(&visualScopeMsg);
    bool callbackCalled = false;

    visualScopeClient.addFunctionToMainCommand("getData", [&](uint8_t num, JsonDocument& doc) {
        callbackCalled = true;
        BOOST_CHECK(0 == doc["roasterID"]);
    });

    visualScopeClient.processEvent(128, JSONPAYLOAD_2, sizeof(JSONPAYLOAD_2));

    BOOST_CHECK(true == callbackCalled);
}

#endif