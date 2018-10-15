/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "CONFIG.h"
#include "CONSOLE.h"
#include "IO.h"

boolean CONFIG::BIOS_INT; //BIOS Intercept flag
uint8_t CONFIG::SENSE_SW = 0x00;
uint32_t CONFIG::RAM_AVAIL = 0x10000L;
bool CONFIG::exitFlag = false;

void CONFIG::init()
{
	Serial.begin(COM_BAUD);
	while (!Serial) {}
	delay(1000);

	CONSOLE::clrscr();
	
	CONSOLE::welcome();
}
