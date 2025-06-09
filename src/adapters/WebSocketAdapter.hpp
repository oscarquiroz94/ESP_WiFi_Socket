#pragma once

#include "Compiletype.hpp"
#include "adapters/ESPadapter.hpp"

#ifdef DEPLOY
#include <WebSocketsServer.h>
#include <WiFi.h>
#else
#include <stdint.h>
#include <iostream>
#include <cstring>
#include <functional>

typedef enum {
    WStype_ERROR,
    WStype_DISCONNECTED,
    WStype_CONNECTED,
    WStype_TEXT,
    WStype_BIN,
    WStype_FRAGMENT_TEXT_START,
    WStype_FRAGMENT_BIN_START,
    WStype_FRAGMENT,
    WStype_FRAGMENT_FIN,
    WStype_PING,
    WStype_PONG,
} WStype_t;

class IPAddress
{
    public:
        String toString() {return "192.168.4.1";}
};

class WiFiClass
{
    public:
        void mode(int mode) {}
        void setSleep(bool sleep) {}
        bool softAP(const char* ssid, const char* password, int channel) { return true; }
        IPAddress softAPIP() {return IPAddress();}
        std::string toString() {return "192.168.4.1";}
        int channel() {return 1;}
        void begin() {}
        void end() {}
        bool isRunning() { return true; }
        void softAPdisconnect(bool) {}
        int scanNetworks() { return 5; }
        String SSID(int i) 
        { 
            if ( i == 0)      return "ROASTER";
            else if ( i == 1) return "ROASTER_1";
            else if ( i == 2) return "ROASTER_2";
            else if ( i == 3) return "ROASTER12345ROASTER";
            else return "Network"; 
        }
};

class WebSocketsServer
{
    public:
        WebSocketsServer(int port) {}
        void begin() {}
        bool isRunning() { return true; }
        void close() {}
        void loop() {}
        void sendTXT(uint8_t num, JsonDocument& doc) {}
        void sendTXT(uint8_t num, String &stream) 
        {
            outputString += stream;
            ESPadapter::serial_print("sendTXT: ");
            ESPadapter::serial_println(outputString);
        }
        void onEvent(std::function<void(uint8_t num, WStype_t type, uint8_t *payload, size_t length)>) {}
        IPAddress remoteIP(uint8_t num) { return IPAddress(); }
        std::string getOutputString() const { return outputString; }
        void cleanOutputString() 
        {
            outputString = "";
            ESPadapter::serial_println("clean");
        }
        uint8_t getOutputNum() const { return outputNum; }

    private:
        std::string outputString = "";
        uint8_t outputNum = 0;
};



#define WIFI_OFF 0
#define WIFI_AP 1

#endif