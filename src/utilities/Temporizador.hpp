#pragma once

#include "ESPadapter.hpp"

class Temporizador {
    public:
        //false, true para iniciar contador desde cero y ejecutar accion
        //true, true para ejecutar accion y luego iniciar contador para otra ejecucion
        //false, false solo iniciar timer despues de darle vida
        //true, false sin efecto
        Temporizador(bool run, bool en, uint8_t name) : 
          _running(run), _enable(en), _name(name) {}

        Temporizador(bool run = false, bool en = true) : 
          _running(run), _enable(en) {}

        virtual inline bool tiempo(unsigned long time){
            if(!_running && _enable) {
                _contador = ESPadapter::millisec();
                _running = true;
            }
            if(ESPadapter::millisec() - _contador >= time && _running && _enable){
                _running = false;
                _lock = true; //bloquear el otro metodo
                return true;
            }
            return false;
        }
        inline bool tiempo(unsigned long time, bool once)
        {
          if(!_lock) return tiempo(time);
          else return false;
        }
        inline void kill() {_enable = false; _running = false;}
        inline void restart() {_running = false;}
        inline void alive() {_enable = true;}
        inline bool getRun() { return _running;}
        inline bool is_alive() {return _enable;}
        inline uint8_t get_name() {return _name;}
        inline void reset() {_contador = ESPadapter::millisec();}
        inline void unlock() {_lock = false;}
        inline unsigned long get_time() 
        {
          if (_running) return ESPadapter::millisec() - _contador;
          else return 0L;
        }

    protected:

        bool _running;
        bool _enable;
        uint8_t _name = 0;
        unsigned long _contador = 0L;
        bool _lock = false;
};