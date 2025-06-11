#pragma once

#include "Temporizador.hpp"

// TODO: TESTING
class Heartbeat
{
    public:
        Heartbeat() 
            : t_setoffline(false, true),
              t_isoffline(false, false) 
            {}

        enum Status {online, offline};
        enum Step {roasting, other};

        void set_status(Status sta) {state = sta;}
        void set_step(Step stp) {step = stp;}
        bool is_alert() {return alert;}

        void loop()
        {
            // Cada 3 segundos poner offline (artisan debera poner online)
            if (t_setoffline.tiempo(3000)) state = offline;

            // Si esta offline y roasting por mas de 5 seg alertar (puede ser 8seg)
            if (step == roasting && state == offline)
                t_isoffline.alive();

            if (step == roasting && state == online)
                t_isoffline.kill();

            if (t_isoffline.tiempo(5000)) 
            {
                alert = true;
                t_isoffline.kill();
            }    
            else alert = false;
        }

    private:
        Status state {offline};
        Step   step  {other};
        bool   alert {false};

        Temporizador t_setoffline;
        Temporizador t_isoffline;
};