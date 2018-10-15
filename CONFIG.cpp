/**
 * Copyright (c) 2018 Roland K�rtvely <roland.kortvely@gmail.com>
 */


#include "CONFIG.h"
#include "CONSOLE.h"
#include "IO.h"

boolean CONFIG::BIOS_INT; //BIOS Intercept flag
uint8_t CONFIG::SENSE_SW = 0x00;

void CONFIG::init()
{
	Serial.begin(COM_BAUD);
	while (!Serial) {}
	delay(1000);

	CONSOLE::clrscr();
	
	CONSOLE::welcome();
}