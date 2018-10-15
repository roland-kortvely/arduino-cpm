/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#ifndef _IO_h
#define _IO_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class IO
{
public:

	static volatile bool exitFlag;

	static uint32_t RAM_AVAIL;

	//AUX
	static bool LED_on;
	static uint8_t LED_count;

	//Other
	static uint8_t SENSE_SW;

	static void init();

	static void _INPORT();
	static void _OUTPORT();
	static uint8_t _getPORT(uint16_t adr);
	static void _setPORT(uint16_t adr, uint8_t dat);
};

#endif
