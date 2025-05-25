#include "SerialPort.hpp"

char SerialPort::comandoFromAtmega[50];

void SerialPort::openPort()
{
    ESPadapter::serial_begin(baudRate);
    ESPadapter::serial_timeout(100);
}

void SerialPort::addFunctionToMainCommand
    (const char* key, std::function<void(const char*)> func)
{
    map2func[key] = func;
}

// Not to change this function, it is not cover by test
void SerialPort::listen()
{
    uint16_t i = 0;
    while (ESPadapter::serial_available())
    {
        uint8_t caracter = ESPadapter::serial_read();
        if (caracter == '\0' || caracter == '\n' || caracter == '\r')
        {
            comandoFromAtmega[i] = '\0';
            break;
        }
        if (i > 48) break;
        comandoFromAtmega[i] = caracter;
        i++;
        ESPadapter::retardo(2);
    }
}

void SerialPort::processEvent()
{
    for (auto it = map2func.begin(); it != map2func.end(); it++)
    {
        if (compare(comandoFromAtmega, it->first))
        {
            it->second(comandoFromAtmega);
            break;
        }
    }
}

int SerialPort::length(const char *cadena)
{
    int i = 0;
    while(cadena[i] != '\0' && cadena[i] != '\r' && cadena[i] != '\n') i++;
    return i;
}

bool SerialPort::compare(const char *cadena, const char *subcadena)
{
    for(int i=0; i < length(subcadena); i++){
        if(cadena[i] != subcadena[i]) 
            return false;
    }
    return true;
}

bool SerialPort::compareEqual(const char *cadena, const char *subcadena)
{
    if(length(cadena) != length(subcadena)) 
        return false;

    for(int i=0; i < length(subcadena); i++){
        if(cadena[i] != subcadena[i]) 
            return false;
    }
    return true;
}