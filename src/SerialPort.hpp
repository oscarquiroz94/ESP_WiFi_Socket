#pragma once

#include "ESPadapter.hpp"
#include <map>
#include <functional>

class SerialPort
{
    public:
        SerialPort(int baudRate) : baudRate(baudRate) {}
        
        void openPort();
        void listen();
        void processEvent();
        void addFunctionToCommand
            (const char* key, std::function<void(const char*)> func);
        
    protected:
        static char comandoFromAtmega[50];
        bool compare(const char *cadena, const char *subcadena);

    private:
        int baudRate;
        
        std::map<const char*, std::function<void(const char*)>> map2func;
        
        int length(const char *cadena);
        bool compareEqual(const char *cadena, const char *subcadena);
};