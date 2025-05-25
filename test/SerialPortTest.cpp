#include "Compiletype.hpp"
#ifdef TEST

#include "SerialPort.hpp"
#include "SerialPortTestAccess.hpp"
#include "CrossSectionalData.hpp"

#include <boost/test/unit_test.hpp>

const char* COMMANDPORT_1 = "S,ROASTER,Clave123*,5,";

BOOST_AUTO_TEST_CASE(given_COMMANDPORT_1_when_EVENTSERIAL_then_SET_SSID_PASS_CHANNEL)
{
    SerialPort serialport(115200);
    CrossSectionalData applicationdata;
    SerialPortTestAccess serialportTestAccess;
    
    bool callbackCalled = false;

    serialport.addFunctionToMainCommand("S,", [&](const char* comand) { 
        callbackCalled = true;

        //Limpiar valores anteriores
        memset(applicationdata.ssidSocket, 0, 50);
        memset(applicationdata.passSocket, 0, 50);

        char* cpycommand = (char*)comand;

        char *lista = strtok(cpycommand, ",");

        lista = strtok(NULL, ",");
        if (lista != NULL) strcpy(applicationdata.ssidSocket, std::string(lista).c_str());

        lista = strtok(NULL, ",");
        if (lista != NULL) strcpy(applicationdata.passSocket, std::string(lista).c_str());

        lista = strtok(NULL, ",");
        if (lista != NULL) applicationdata.canalwifi = static_cast<uint8_t>(std::stoi(lista));
        
        const char* ssidSocket = "ROASTER";
        const char* passSocket = "Clave123*";
        uint8_t canalwifi = 5;
        BOOST_CHECK(true == serialportTestAccess.compare(applicationdata.ssidSocket, ssidSocket));
        BOOST_CHECK(true == serialportTestAccess.compare(applicationdata.passSocket, passSocket));
        BOOST_CHECK(applicationdata.canalwifi == canalwifi);
    });

    serialportTestAccess.setCommandFromAtmega(COMMANDPORT_1);
    serialport.processEvent();

    BOOST_CHECK(true == callbackCalled);
}

#endif