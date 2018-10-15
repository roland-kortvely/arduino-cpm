/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "CMD.h"
#include "CONFIG.h"
#include "CONSOLE.h"
#include "FDD.h"
#include "SD.h"
#include "RAM.h"
#include "i8080.h"
#include "BIOS.h"
#include "IPL.h"

void CMD::exec()
{
	CONSOLE::clrarea();

	if (CONSOLE::mon_buffer[0] == 'X') {
		CMD::format();
		return;
	}

	if (CONSOLE::mon_buffer[0] == 'V') {
		CMD::status();
		return;
	}

	if (CONSOLE::mon_buffer[0] == 'M') {
		CMD::MEM_TEST();
		return;
	}

	if (CONSOLE::mon_buffer[0] == 'C') {
		CMD::BIOS();
		return;
	}

	CONSOLE::color(1);
	CONSOLE::print(F("INVALID COMMAND!"));
	CONSOLE::color(9);
}

void CMD::BIOS() {
	CONSOLE::clrscr();						//clear screen

	if (!BIOS::IPL()) {						//initial loader
		CONSOLE::error("BIOS::IPL FAILURE!");
	}

	CONFIG::BIOS_INT = true;				//BIOS intercept enabled

	CONSOLE::MON = false;

	IPL::call(_BIOS);						//JMP TO BIOS

	CONSOLE::MON = true;
}

void CMD::MEM_TEST() {
	CONSOLE::color(2);
	CONSOLE::savecur();
	CONSOLE::xy(0, 0);
	RAM::MEM_TEST();
	CONSOLE::loadcur();
	CONSOLE::color(9);
}

void CMD::status() {
	CONSOLE::savecur();
	CONSOLE::xy(0, 0);
	I8080::state();
	CONSOLE::loadcur();
}

void CMD::format() {

	uint8_t driveno;
	uint32_t start;
	uint8_t res;

	driveno = 0xFF;

	switch (CONSOLE::mon_buffer[1]) {
		case 'A'...char(uint8_t('A') + FDD_NUM - 1) : driveno = (uint8_t(CONSOLE::mon_buffer[1]) - uint8_t('A'));
			break;
	}

	if (driveno == 0xFF) {
		CONSOLE::color(1);
		CONSOLE::println(F("Invalid disk!"));
		CONSOLE::color(9);
		return;
	}

	CONSOLE::println(F("Format disk"));
	CONSOLE::println();

	//format
	start = FDD::SD_FDD_OFFSET[driveno];
	for (uint32_t i = 0; i < SD_BLK_SIZE; i++) {
		SD::_dsk_buffer[i] = CPM_EMPTY;
	}

	for (uint32_t i = 0; i < DISK_SIZE*TRACK_SIZE; i++) {
		CONSOLE::print("\r");
		CONSOLE::print(F("SECTOR "));
		Serial.print(i, DEC);
		res = SD::sd.writeBlock(i + start, SD::_dsk_buffer);
	}

	CONSOLE::ok();
}
