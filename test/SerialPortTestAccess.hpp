#pragma once

#include "serial/SerialPort.hpp"

class SerialPortTestAccess : public SerialPort
{
    public:
        SerialPortTestAccess() : SerialPort(115200) {}

        void setCommandFromAtmega(const char* command)
        {
            memset(comandoFromAtmega, 0, sizeof(comandoFromAtmega));
            strcpy(comandoFromAtmega, command);
        }

        bool compareEqual(const char *cadena, const char *subcadena)
        {
            return SerialPort::compareEqual(cadena, subcadena);
        }
};