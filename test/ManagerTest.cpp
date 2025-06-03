#include "Compiletype.hpp"
#ifdef TEST

#include "ManagerAccess.hpp"
#include "SerialPortTestAccess.hpp"
#include <boost/test/unit_test.hpp>

const char *SERIALCOMMAND_1 = R"(S,ROASTER,Clave123*,1,)";
const char *EXPECTED_1 = "CSOIPS192.168.4.1CH1SID,ROASTER_3,Clave123*,1";

const char *SERIALCOMMAND_2 = R"(S,ROASTER_4,Clave123*,1,)";
const char *EXPECTED_2 = "CSOIPS192.168.4.1CH1SID,ROASTER_4,Clave123*,1";

BOOST_AUTO_TEST_CASE(given_SERIALCOMMAND_1_when_REGISTER_SERIAL_PORT_HANDLER_then_EXPECTED_1)
{
    SerialPortTestAccess serialportTestAccess;
    ManagerAccess manager;
    manager.setMaxTimeSearch(100);
    ESPadapter::clearSerialOutput();
    
    manager.registerSerialPortHandler();

    serialportTestAccess.setCommandFromAtmega(SERIALCOMMAND_1);
    manager.processEvent();

    std::string captured = ESPadapter::getSerialOutput();

    // ROASTER is already registered on Network test
    // So the expected result is ROASTER_3
	BOOST_CHECK_EQUAL(captured.c_str(), EXPECTED_1);

    ESPadapter::serial_println();
}

BOOST_AUTO_TEST_CASE(given_SERIALCOMMAND_2_when_REGISTER_SERIAL_PORT_HANDLER_then_EXPECTED_2)
{
    SerialPortTestAccess serialportTestAccess;
    ManagerAccess manager;
    manager.setMaxTimeSearch(100);
    ESPadapter::clearSerialOutput();
    
    manager.registerSerialPortHandler();

    serialportTestAccess.setCommandFromAtmega(SERIALCOMMAND_2);
    manager.processEvent();

    std::string captured = ESPadapter::getSerialOutput();

    // ROASTER_4 is not registered on Network test
    // So the expected result is ROASTER_4
	BOOST_CHECK_EQUAL(captured.c_str(), EXPECTED_2);

    ESPadapter::serial_println();
}

#endif