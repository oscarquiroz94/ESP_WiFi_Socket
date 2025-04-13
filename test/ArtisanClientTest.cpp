#include "Compiletype.hpp"

#ifdef TEST

#define BOOST_TEST_MODULE ArtisanClientTest

#include "ArtisanClient.hpp"
#include <boost/test/included/unit_test.hpp>

const char* JSONPAYLOAD1 = R"({"roasterID": 1, "command": "setControlParams", "params": {"aire": 100, "tambor": 80, "quemador": 60, "soplador": 50}})";

BOOST_AUTO_TEST_CASE(given_JSONPAYLOAD1_when_EVENTWB_then_SETPARAMS)
{
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

    artisanClient.processEvent(128, JSONPAYLOAD1, sizeof(JSONPAYLOAD1));

    BOOST_CHECK(true == callbackCalled);
}

#endif