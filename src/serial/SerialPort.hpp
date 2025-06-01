#pragma once

#include "adapters/ESPadapter.hpp"
#include <map>
#include <functional>

class SerialPort
{
    public:
        SerialPort(int baudRate) : baudRate(baudRate) {}
        
        void openPort();
        void listen();
        void processEvent();

        template<class T> void sendRawData(const T data);

        void addFunctionToMainCommand
            (const char* key, std::function<void(const char*)> func);
        
    protected:
        static char comandoFromAtmega[50];
        

    private:
        unsigned long baudRate;
        
        std::map<const char*, std::function<void(const char*)>> map2func;
        
        int length(const char *cadena);
        bool compareEqual(const char *cadena, const char *subcadena);
        bool compare(const char *cadena, const char *subcadena);

        friend class SerialPortTestAccess; // For testing purposes
};

template<class T>
void SerialPort::sendRawData(const T data)
{
    ESPadapter::serial_print(data);
}