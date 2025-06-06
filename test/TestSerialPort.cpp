#include "Compiletype.hpp"
#ifdef TEST

#include "serial/SerialPort.hpp"
#include "SerialPortTestAccess.hpp"
#include "clients/ArtisanClient.hpp"
#include "messages/ArtisanMessage.hpp"
#include "CrossSectionalData.hpp"

#include <boost/test/unit_test.hpp>

const char* COMMANDPORT_1 = "S,ROASTER,Clave123*,5,";
const char* COMMANDPORT_2 = "MCA";
const char* COMMANDPORT_3 = "IN,230,160,1500,2000,200,600,200";

BOOST_AUTO_TEST_CASE(given_COMMANDPORT_1_when_EVENTSERIAL_S_then_SET_SSID_PASS_CHANNEL)
{
    SerialPort serialport(115200);
    CrossSectionalDataEEPROM applicationdata;
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
        BOOST_CHECK(true == serialportTestAccess.compareEqual(applicationdata.ssidSocket, ssidSocket));
        BOOST_CHECK(true == serialportTestAccess.compareEqual(applicationdata.passSocket, passSocket));
        BOOST_CHECK(applicationdata.canalwifi == canalwifi);
    });

    serialportTestAccess.setCommandFromAtmega(COMMANDPORT_1);
    serialport.processEvent();

    BOOST_CHECK(true == callbackCalled);
}


BOOST_AUTO_TEST_CASE(given_COMMANDPORT_1_when_EVENTSERIAL_MCA_then_SEND_STARTROASTING)
{
    SerialPort serialport(115200);
    CrossSectionalDataEEPROM applicationdata;
    SerialPortTestAccess serialportTestAccess;

    ArtisanMessage artisanMsg;
    ArtisanClient artisanClient(&artisanMsg);

    bool callbackCalled = false;

    serialport.addFunctionToMainCommand("MCA", [&](const char* comand){
        String output;
        JsonDocument outdoc;
        int8_t idArtisan = artisanClient.getClientId();

        outdoc["pushMessage"] = "startRoasting";
        serializeJson(outdoc, output);
        
        const char* expected = "{\"pushMessage\":\"startRoasting\"}";
        const char* outputchar = output.c_str();
        BOOST_CHECK(true == serialportTestAccess.compareEqual(expected, outputchar));
        callbackCalled = true;
    });

    serialportTestAccess.setCommandFromAtmega(COMMANDPORT_2);
    serialport.processEvent();

    BOOST_CHECK(true == callbackCalled);
}

BOOST_AUTO_TEST_CASE(given_COMMANDPORT_1_when_EVENTSERIAL_IN_then_SET_APPLICATION_DATA)
{
    SerialPort serialport(115200);
    CrossSectionalData applicationdata;
    SerialPortTestAccess serialportTestAccess;

    bool callbackCalled = false;

    // IN,230,160,1500,2000,200,600,200 --> ET, BT, Q, T, S, ROR, delta
    serialport.addFunctionToMainCommand("IN,", [&](const char* comand) { 
        char* cpycommand = (char*)comand;
        char *lista = strtok(cpycommand, ",");

		lista = strtok(NULL, ",");
		applicationdata.tempET = ESPadapter::str2int(lista); 

		lista = strtok(NULL, ",");
		applicationdata.tempBT = ESPadapter::str2int(lista);

		lista = strtok(NULL, ",");
		applicationdata.porcentQuem = ESPadapter::str2int(lista);;

		lista = strtok(NULL, ",");
		applicationdata.porcentTamb = ESPadapter::str2int(lista);;

		lista = strtok(NULL, ",");
		applicationdata.porcentSopl = ESPadapter::str2int(lista);;

		lista = strtok(NULL, ",");
		applicationdata.RoR = ESPadapter::str2int(lista);

		lista = strtok(NULL, ",");
		applicationdata.deltaETBT = ESPadapter::str2int(lista);

        callbackCalled = true;

        BOOST_CHECK(230 == applicationdata.tempET);
        BOOST_CHECK(160 == applicationdata.tempBT);
        BOOST_CHECK(1500 == applicationdata.porcentQuem);
        BOOST_CHECK(2000 == applicationdata.porcentTamb);
        BOOST_CHECK(200 == applicationdata.porcentSopl);
        BOOST_CHECK(600 == applicationdata.RoR);
        BOOST_CHECK(200 == applicationdata.deltaETBT);
    });

    serialportTestAccess.setCommandFromAtmega(COMMANDPORT_3);
    serialport.processEvent();

    BOOST_CHECK(true == callbackCalled);
}

#endif