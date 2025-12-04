#pragma once

#include "Temporizador.hpp"

class Heartbeat
{
    public:
        Heartbeat() 
            : t_setoffline(false, true),
              t_isoffline(false, false) 
            {}

        enum Status {online, offline};
        enum Step {ready, roasting, endroasting, noready};

        void set_status(Status sta) {state = sta;}
        void set_step(Step stp) {step = stp;}
        bool is_alert() {return alert;}
        void set_alert(bool state) {alert = state;}

        void loop()
        {
            // Cada 3 segundos poner offline (visualScope debera poner online)
            if (t_setoffline.tiempo(3000)) state = offline;

            if (state == online)
                t_isoffline.alive();
                
            if (step == roasting && state == offline)
                t_isoffline.alive();

            if (step == endroasting && state == offline)
                t_isoffline.alive();

            if (step == ready && state == offline)
                t_isoffline.alive();

            //---------

            if (step == noready && state == offline)
                t_isoffline.kill();

            if (step == roasting && state == online)
                t_isoffline.kill();

            if (step == endroasting && state == online)
                t_isoffline.kill();

            if (step == ready && state == online)
                t_isoffline.kill();

            if (t_isoffline.tiempo(5000)) 
            {
                alert = true;
                t_isoffline.kill();
                ESPadapter::debug_print("Heartbeat step: ");
                ESPadapter::debug_print((uint16_t)step);
                ESPadapter::debug_print(" status: ");
                ESPadapter::debug_println((uint16_t)state);
            } 
        }

    private:
        Status state {offline};
        Step   step  {noready};
        bool   alert {false};

        Temporizador t_setoffline;
        Temporizador t_isoffline;
};