#include "Compiletype.hpp"
#ifdef TEST

#include "ArtisanClient.hpp"
#include <boost/test/unit_test.hpp>

// TODO: JSONPAYLOAD_2 = {"command":"setControlParams","params":{"quemador":63},"id":26764,"roasterID":0}

BOOST_AUTO_TEST_CASE(given_JSONPAYLOAD_1_when_EVENTWB_then_SETPARAMS)
{
    const char* JSONPAYLOAD_1 = R"({"roasterID": 1, "command": "setControlParams", "params": {"aire": 100, "tambor": 80, "quemador": 60, "soplador": 50}})";

    ArtisanClient artisanClient;
    bool callbackCalled = false;

    artisanClient.addFunctionToCommand("setControlParams", [&](uint8_t num, JsonDocument& doc) {
        callbackCalled = true;
        BOOST_CHECK(1 == doc["roasterID"]);
        BOOST_CHECK(100 == doc["params"]["aire"]);
        BOOST_CHECK(80 == doc["params"]["tambor"]);
        BOOST_CHECK(60 == doc["params"]["quemador"]);
        BOOST_CHECK(50 == doc["params"]["soplador"]);
    });

    artisanClient.processEvent(128, JSONPAYLOAD_1, sizeof(JSONPAYLOAD_1));

    BOOST_CHECK(true == callbackCalled);
}


// Cases for single command are cover here 
BOOST_AUTO_TEST_CASE(given_JSONPAYLOAD_2_when_EVENTWB_then_GETDATA)
{
    const char* JSONPAYLOAD_2 = R"({"command":"getData","id":92295,"roasterID":0})";

    ArtisanClient artisanClient;
    bool callbackCalled = false;

    artisanClient.addFunctionToCommand("getData", [&](uint8_t num, JsonDocument& doc) {
        callbackCalled = true;
        BOOST_CHECK(1 == doc["roasterID"]);
    });

    artisanClient.processEvent(128, JSONPAYLOAD_2, sizeof(JSONPAYLOAD_2));

    BOOST_CHECK(true == callbackCalled);
}

#endif