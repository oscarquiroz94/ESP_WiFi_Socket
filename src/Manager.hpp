#pragma once

#include "compiletype.hpp"

#include "serial/SerialPort.hpp"
#include "websocket/WebsocketManager.hpp"
#include "websocket/WebsocketClientHandler.hpp"
#include "CrossSectionalData.hpp"
#include "clients/GenericClient.hpp"
#include "messages/VisualScopeMessage.hpp"
#include "messages/AudioCrackMessage.hpp"
#include "websocket/PairingManager.hpp"
#include "utilities/Heartbeat.hpp"

/*
* @brief Class Manager
* 
* This class is the monolithic process of the application. 
* It register the serial port and websocket handlers, and manages the
* communication between the different components of the application.
* Therefore, could have many changes in the future.
*/
class Manager
{
    public:

        Manager();

        void initialize();
        void run();

    private:
        CrossSectionalData       m_applicationdata;
        CrossSectionalDataEEPROM m_eepromdata;
        SerialPort               m_serialport;
        WebSocketsServer         m_webSocket;
        VisualScopeMessage       m_visualScopeMsg;
        AudioCrackMessage        m_audioCrackMsg;
        WebsocketClientHandler   m_clientHandler;
        GenericClient            m_visualScopeClient;
        GenericClient            m_audioCrackClient;
        PairingManager           m_peer;
        Heartbeat                m_beat;
        uint8_t                  m_sendVersionAmount = 0;
        uint32_t                 m_tempo_sendversion = 0;
        uint32_t                 m_versionESP = 0;
        bool                     m_heartbeatonce = false;

        void registerSerialPortHandler();
        void registerWebSocketHandler();
        void registerVisualScope();
        void registerAudioCrack();
        void send_data();

        friend class ManagerAccess;

};