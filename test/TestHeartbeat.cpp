#include "Compiletype.hpp"

#ifdef TEST
#include <boost/test/unit_test.hpp>
#include "utilities/Heartbeat.hpp"
#include <thread>
#include <mutex>
#include <chrono>
#include <string>

namespace TestHearbeat
{
    std::string sharedCommand = "";
    std::mutex mtx;

    void setCommand(const std::string &txt)
    {
        std::lock_guard<std::mutex> lock(mtx);
        sharedCommand = txt;
    };
    std::string getCommand()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return sharedCommand;
    };
}

BOOST_AUTO_TEST_CASE(given_OFFLINE_ROASTING_when_MORE_THAN_5_SEG_then_IS_ALERT)
{
    ESPadapter::serial_println("testing hearbeat...");

    std::thread artisanThreat([]{

        //Enviar 10 comandos cada segundo
        for (int i = 0; i < 10; i++)
        {
            TestHearbeat::setCommand("getData");
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        //Deja de enviar
        std::this_thread::sleep_for(std::chrono::seconds(10));

        //Enviar 10 comandos cada segundo otra vez
        for (int i = 0; i < 10; i++)
        {
            TestHearbeat::setCommand("getData");
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        TestHearbeat::setCommand("fin");
    });

    std::thread roasterThreat([]{
        Heartbeat beat;
        beat.set_step(Heartbeat::roasting);
        std::string bufferCommand = "";

        while (true)
        {
            bufferCommand = TestHearbeat::getCommand();
            TestHearbeat::setCommand("");

            if (bufferCommand == "getData")
            {
                beat.set_status(Heartbeat::online);
                //std::cout << "recibido" << std::endl;
            }

            beat.loop();

            if (beat.is_alert())
            {
                //std::cout << "alerta" << std::endl;
            }

            if (bufferCommand == "fin") break;

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });

    artisanThreat.join();
    roasterThreat.join();
}

#endif