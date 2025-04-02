#pragma once

#include "Compiletype.hpp"

#ifdef DEPLOY
#include <Arduino.h>
#endif

class ESPadapter
{
    public:

    static inline void serial_print(double num, uint16_t precision)
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

	//---------------------

	static inline void serial_print(const __FlashStringHelper *text)
	{
#ifdef DEPLOY
		Serial.print(text);
#else
		char buffer[100];
		strncpy(buffer, (const char *)text, 100);
		std::cout << buffer;
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

	//---------------------

	static inline void serial_println(double value, uint16_t precision)
	{
#ifdef DEPLOY
		Serial.println(value, precision);
#else
		std::cout << value << std::endl;
#endif
	}

	//---------------------

	static inline void serial_println(const __FlashStringHelper *text)
	{
#ifdef DEPLOY
		Serial.println(text);
#else
		char buffer[100];
		strncpy(buffer, (const char *)text, 100);
		std::cout << buffer << std::endl;
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
};