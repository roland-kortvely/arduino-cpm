/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "IPL.h"
#include "CONSOLE.h"
#include "IO.h"
#include "MEM.h"
#include "SD.h";
#include "FDD.h"

void IPL::init()
{

	CONFIG::init();

	Serial.println(F("INITIAL PROGRAM LOAD (IPL)\n"));

	uint8_t res;
	uint8_t LRC;
	uint8_t bank;
	uint8_t block;
	uint8_t CHECKED_BANKS;
	uint32_t blk;
	uint32_t blk_end;
	uint32_t start_time;
	uint32_t _cardsize;

	bool RAMTestPass = true;

	IO::init();

	/*
	cli();

	//Timer
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1  = 0;
	OCR1A  = 312;							// 50Hz
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS12) | (1 << CS10);
	TIMSK1 |= (1 << OCIE1A);

	sei();
	*/

	MEM::init();
	SD::init();
	FDD::init();

	Serial.println("IPL DONE");
}
