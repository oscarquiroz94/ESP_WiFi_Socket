#pragma once

#include "SerialPort.hpp"

class SerialPortTestAccess : public SerialPort
{
    public:
        SerialPortTestAccess() : SerialPort(115200) {}

        void setCommandFromAtmega(const char* command)
        {
            memset(comandoFromAtmega, 0, sizeof(comandoFromAtmega));
            strcpy(comandoFromAtmega, command);
        }

        bool compare(const char *cadena, const char *subcadena)
        {
            return SerialPort::compare(cadena, subcadena);
        }
};