#pragma once

#include "Compiletype.hpp"
#include <ArduinoJson.h>

#ifdef DEPLOY
#include <Arduino.h>
#else
#include <stdint.h>
#include <iostream>
#include <cstring>
#include <chrono>
#include <thread>
#include <windows.h>
#include <string>
#endif


#ifndef DEPLOY
using String = std::string;
#endif

class ESPadapter
{
    public:

	static inline long str2int(const char *txt)
	{
#ifdef DEPLOY
		if (txt != nullptr) return String(txt).toInt();
		else return 0;
#else
		if (txt != nullptr) return std::stoi(txt);
		else return 0;
#endif
	}

	static inline void print_null(std::string nameclass, const char* function)
	{
#ifdef DEPLOY
		Serial.print(nameclass.c_str());
		Serial.print("::");
		Serial.print(function);
		Serial.println(" - NULL");
#else
		std::cout << nameclass << "::" << function << " - NULL" << std::endl;
#endif
	}

	static inline unsigned long millisec()
	{
#ifdef DEPLOY
		return millis();
#else
		auto duration = std::chrono::system_clock::now().time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		return millis;
#endif
	}

	static inline void serial_begin(unsigned long baudRate)
	{
#ifdef DEPLOY
		Serial.begin(baudRate);
#endif
	}

	static inline void serial_timeout(uint16_t timeout)
	{
#ifdef DEPLOY
		Serial.setTimeout(timeout);
#endif
	}

    static inline void serial_print(double num, uint16_t precision = 2)
	{
#ifdef DEPLOY
		Serial.print(num, precision);
#else
		std::cout << (uint16_t)num;
#endif
	}

	//----------------------

	static inline void serial_print(const uint8_t num)
	{
#ifdef DEPLOY
		Serial.print(num);
#else
		std::cout << (uint16_t)num;
#endif
	}

	//-------------------

	template <class TipodatoSerial>
	static inline void serial_print(const TipodatoSerial text)
	{
#ifdef DEPLOY
		Serial.print(text);
#else
		std::cout << text;
#endif
	}

	//----------------------

	static inline void serial_println()
	{
#ifdef DEPLOY
		Serial.println();
#else
		std::cout << std::endl;
#endif
	}

	//----------------------

	static inline void serial_println(const uint8_t num)
	{
#ifdef DEPLOY
		Serial.println(num);
#else
		std::cout << (uint16_t)num << std::endl;
#endif
	}

	//-----------------------

	template <class TipodatoSerial>
	static inline void serial_println(const TipodatoSerial text)
	{
#ifdef DEPLOY
		Serial.println(text);
#else
		std::cout << text << std::endl;
#endif
	}

	static inline void serial_println(const String &text)
	{
#ifdef DEPLOY
		Serial.println(text);
#else
		std::cout << text.c_str() << std::endl;
#endif
	}

	//---------------------

	static inline void serial_println(double value, uint16_t precision)
	{
#ifdef DEPLOY
		Serial.println(value, precision);
#else
		std::cout << value << std::endl;
#endif
	}

	//----------------------

	static inline bool serial_available()
	{
#ifdef DEPLOY
		return Serial.available();
#else
		return true;
#endif
	}

	//----------------------

	static inline int serial_read()
	{
#ifdef DEPLOY
		return Serial.read();
#else
		return 0;
#endif
	}

	static inline void serial_flush()
	{
#ifdef DEPLOY
		Serial.flush();
#endif
	}

	//************************************/

	static inline void retardo(const uint16_t t)
	{
#ifdef DEPLOY
		delay(t);
#else
		std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(t));
#endif
	}

	static inline unsigned long milliseconds()
	{
#ifdef DEPLOY
		return millis();
#else
		auto duration = std::chrono::system_clock::now().time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		return millis;
#endif
	}
};