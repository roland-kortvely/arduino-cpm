/**
 * Copyright (c) 2018 Roland K�rtvely <roland.kortvely@gmail.com>
 */


#include "IPL.h"
#include "CONSOLE.h"
#include "MEM.h"
#include "FDD.h"

void IPL::init()
{

	uint8_t k;
	uint8_t res;
	uint8_t LRC;
	uint8_t bank;
	uint8_t block;
	uint8_t CHECKED_BANKS;
	uint16_t j;
	uint32_t blk;
	uint32_t blk_end;
	uint32_t start_time;
	uint32_t i;
	uint32_t _cardsize;

	bool RAMTestPass = true;

	CONSOLE::clrscr();

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

	CONSOLE::welcome();

	MEM::init();
	FDD::init();

	CONSOLE::con_flush();
}
