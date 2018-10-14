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

	static volatile uint8_t  _DB; //data bus buffer
	static volatile uint16_t _AB; //data bus buffer

	static volatile bool exitFlag;

	static uint32_t RAM_AVAIL;

	//AUX
	static bool LED_on;
	static uint8_t LED_count;
	static uint8_t LED_delay;
	static uint8_t LED_pin;		
	static uint8_t IN_pin;		
	static uint8_t OUT_pin;	
	static uint8_t IN_PORT;		
	static uint8_t OUT_PORT;	

	//Other
	static uint8_t SENSE_SW;
	static uint8_t SENSE_SW_PORT;


	static void init();
};

#endif
