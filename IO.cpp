/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "IO.h"

volatile bool IO::exitFlag = false;

uint32_t IO::RAM_AVAIL = 0x10000L;

//AUX
bool    IO::LED_on = false;
uint8_t IO::LED_count;
uint8_t IO::LED_delay = 3;
uint8_t IO::LED_pin = 9;		//D9 pin - LED
uint8_t IO::IN_pin = 8;			//D8 pin - IN
uint8_t IO::OUT_pin = 7;		//D7 pin - OUT
uint8_t IO::IN_PORT = 0xF0;		//IN port
uint8_t IO::OUT_PORT = 0xF1;	//OUT port

//Other
uint8_t IO::SENSE_SW = 0x00;
uint8_t IO::SENSE_SW_PORT = 0xFF;


void IO::init()
{
	pinMode(IO::LED_pin, OUTPUT);
	digitalWrite(IO::LED_pin, LOW);
	pinMode(IO::IN_pin, INPUT);
	pinMode(IO::OUT_pin, OUTPUT);
	digitalWrite(IO::OUT_pin, LOW);
}

