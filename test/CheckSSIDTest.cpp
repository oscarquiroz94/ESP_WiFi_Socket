#include "Compiletype.hpp"
#ifdef TEST

#include "utilities/CheckSSID.hpp"
#include "CrossSectionalData.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(given_ROASTER_when_SSID_ALREADY_EXIST_then_SET_ROASTER_1)
{
    const char *SSID_1 = "ROASTER"; 
    CrossSectionalDataEEPROM eepromdata;

    memset(eepromdata.ssidSocket, 0, sizeof(eepromdata.ssidSocket));
    strcpy(eepromdata.ssidSocket, SSID_1);
    CheckSSID::validateSSID(eepromdata);

    // ROASTER, ROASTER_1 and ROASTER_2 already exists on Network test
    // So the expected result is ROASTER_3
    BOOST_CHECK_EQUAL(std::string(eepromdata.ssidSocket), "ROASTER_3");
}

BOOST_AUTO_TEST_CASE(given_ROASTER_1_when_SSID_ALREADY_EXIST_then_SET_ROASTER_2)
{
    const char *SSID_1 = "ROASTER_1";
    CrossSectionalDataEEPROM eepromdata;

    memset(eepromdata.ssidSocket, 0, sizeof(eepromdata.ssidSocket));
    strcpy(eepromdata.ssidSocket, SSID_1);
    CheckSSID::validateSSID(eepromdata);

    // ROASTER, ROASTER_1 and ROASTER_2 already exists on Network test
    // So the expected result is ROASTER_3
    BOOST_CHECK_EQUAL(std::string(eepromdata.ssidSocket), "ROASTER_3");
}


BOOST_AUTO_TEST_CASE(given_ROASTER12345ROASTER_when_SSID_ALREADY_EXIST_AND_TO_BIG_then_SET_REMOVE_LAST_TWO_CHARS)
{
    const char *SSID_1 = "ROASTER12345ROASTER";
    CrossSectionalDataEEPROM eepromdata;

    memset(eepromdata.ssidSocket, 0, sizeof(eepromdata.ssidSocket));
    strcpy(eepromdata.ssidSocket, SSID_1);
    CheckSSID::validateSSID(eepromdata);

    // ROASTER12345ROASTER already exists on Network test
    // So the expected result is ROASTER12345ROAST_1 because the last two characters are removed
    // due to max length of SSID
    BOOST_CHECK_EQUAL(std::string(eepromdata.ssidSocket), "ROASTER12345ROAST_1");
}

#endif