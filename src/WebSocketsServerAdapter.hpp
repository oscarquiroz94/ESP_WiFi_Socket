#pragma once

#include "Compiletype.hpp"

#ifdef DEPLOY
#include <WebSocketsServer.h>
#include <WiFi.h>
#else
#include <stdint.h>
#include <iostream>
#include <cstring>

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
        std::string toString() {return "";}
};

class WiFiClass
{
    public:
        void mode(int mode) {}
        void setSleep(bool sleep) {}
        bool softAP(const char* ssid, const char* password, int channel) { return true; }
        void begin() {}
        void end() {}
        bool isRunning() { return true; }
};

class WebSocketsServer
{
    public:
        WebSocketsServer(int port) {}
        void begin() {}
        bool isRunning() { return true; }
        void close() {}
        void loop() {}
        IPAddress remoteIP(uint8_t num) { return IPAddress(); }
};



#define WIFI_OFF 0
#define WIFI_AP 1

#endif