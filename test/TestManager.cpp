#include "Compiletype.hpp"
#ifdef TEST

#include "ManagerAccess.hpp"
#include "SerialPortTestAccess.hpp"
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(given_S_ROASTER_when_REGISTER_SERIAL_PORT_HANDLER_then_EXPECTED_1)
{
    const char *SERIALCOMMAND = R"(S,ROASTER,Clave123*,1,)";
    const char *EXPECTED      = "CSOIPS192.168.4.1CH1SID,ROASTER_3,Clave123*,1";

    SerialPortTestAccess serialportTestAccess;
    ManagerAccess manager;
    manager.setMaxTimeSearch(100);
    ESPadapter::clearSerialOutput();
    
    manager.registerSerialPortHandler();

    serialportTestAccess.setCommandFromAtmega(SERIALCOMMAND);
    manager.processEvent();

    std::string captured = ESPadapter::getSerialOutput();

    // ROASTER is already registered on Network test
    // So the expected result is ROASTER_3
	BOOST_CHECK_EQUAL(captured.c_str(), EXPECTED);

    ESPadapter::serial_println();
}


BOOST_AUTO_TEST_CASE(given_S_ROASTER_4_when_REGISTER_SERIAL_PORT_HANDLER_then_EXPECTED)
{
    const char *SERIALCOMMAND = R"(S,ROASTER_4,Clave123*,1,)";
    const char *EXPECTED      = "CSOIPS192.168.4.1CH1SID,ROASTER_4,Clave123*,1";

    SerialPortTestAccess serialportTestAccess;
    ManagerAccess manager;
    manager.setMaxTimeSearch(100);
    ESPadapter::clearSerialOutput();
    
    manager.registerSerialPortHandler();

    serialportTestAccess.setCommandFromAtmega(SERIALCOMMAND);
    manager.processEvent();

    std::string captured = ESPadapter::getSerialOutput();

    // ROASTER_4 is not registered on Network test
    // So the expected result is ROASTER_4
	BOOST_CHECK_EQUAL(captured.c_str(), EXPECTED);

    ESPadapter::serial_println();
}


BOOST_AUTO_TEST_CASE(given_MCA_when_REGISTER_SERIAL_PORT_HANDLER_then_EXPECTED)
{
    const char *SERIALCOMMAND = R"(MCA)";
    const char *JSONPAYLOAD   = R"({"pushMessage":"startRoasting"})";

    SerialPortTestAccess serialportTestAccess;
    ManagerAccess manager;
    WebSocketsServer& ws = manager.getWsReference();
    manager.setMaxTimeSearch(100);
    
    manager.registerSerialPortHandler();

    serialportTestAccess.setCommandFromAtmega(SERIALCOMMAND);
    manager.processEvent();

	BOOST_CHECK_EQUAL(ws.getOutputString().c_str(), JSONPAYLOAD);
}


BOOST_AUTO_TEST_CASE(given_MDR_when_REGISTER_SERIAL_PORT_HANDLER_then_EXPECTED)
{
    const char *SERIALCOMMAND = R"(MDR)";
    const char *JSONPAYLOAD   = R"({"pushMessage":"endRoasting"})";

    SerialPortTestAccess serialportTestAccess;
    ManagerAccess manager;
    WebSocketsServer& ws = manager.getWsReference();
    manager.setMaxTimeSearch(100);
    
    manager.registerSerialPortHandler();

    serialportTestAccess.setCommandFromAtmega(SERIALCOMMAND);
    manager.processEvent();

	BOOST_CHECK_EQUAL(ws.getOutputString().c_str(), JSONPAYLOAD);
}


BOOST_AUTO_TEST_CASE(given_MFC_when_REGISTER_SERIAL_PORT_HANDLER_then_EXPECTED)
{
    const char *SERIALCOMMAND = R"(MFC)";
    const char *JSONPAYLOAD   = R"({"pushMessage":"addEvent","data":{"event":"firstCrackBeginningEvent"}})";
    
    SerialPortTestAccess serialportTestAccess;
    ManagerAccess manager;
    WebSocketsServer& ws = manager.getWsReference();
    manager.setMaxTimeSearch(100);
    
    manager.registerSerialPortHandler();

    serialportTestAccess.setCommandFromAtmega(SERIALCOMMAND);
    manager.processEvent();

	BOOST_CHECK_EQUAL(ws.getOutputString().c_str(), JSONPAYLOAD);
}

#endif