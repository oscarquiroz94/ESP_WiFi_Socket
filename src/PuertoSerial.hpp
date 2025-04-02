#pragma once

#include "ESPadapter.hpp"
#include <map>
#include <functional>

class PuertoSerial
{
    public:
        PuertoSerial(int baudRate) : baudRate(baudRate) {}
        
        void openPort()
        {
            Serial.begin(baudRate);
            Serial.setTimeout(100);
        }

        void addFunctionToCommand(const char* key, std::function<void(const char*)> func)
        {
            map2func[key] = func;
        }

        void listen()
        {
            uint16_t i = 0;
            while (Serial.available())
            {
                uint8_t caracter = Serial.read();
                if (caracter == '\0' || caracter == '\n' || caracter == '\r')
                {
                    comandoFromAtmega[i] = '\0';
                    break;
                }
                if (i > 48)
                    break;
                comandoFromAtmega[i] = caracter;
                i++;
                delay(2);
            }
        }

        void processEvent()
        {
            for (auto it = map2func.begin(); it != map2func.end(); it++)
            {
                // Procesar subcadenas y que la funcion decida
                if (comparar(comandoFromAtmega, it->first) == 0)
                {
                    it->second(comandoFromAtmega);
                    break;
                }
            }
        }

    private:
        int baudRate;
        std::map<const char*, std::function<void(const char*)>> map2func;
        char comandoFromAtmega[50];

        

        int longitud(const char *cadena)
        {
            int i = 0;
            while(cadena[i] != '\0' && cadena[i] != '\r' && cadena[i] != '\n') i++;
            return i;
        }

        bool comparar(const char *cadena, const char *subcadena)
        {
            for(int i=0; i < longitud(subcadena); i++){
                if(cadena[i] != subcadena[i]) 
                    return false;
            }
            return true;
        }

        bool compararEqual(const char *cadena, const char *subcadena)
        {
            if(longitud(cadena) != longitud(subcadena)) 
                return false;

            for(int i=0; i < longitud(subcadena); i++){
                if(cadena[i] != subcadena[i]) 
                    return false;
            }
            return true;
        }
};