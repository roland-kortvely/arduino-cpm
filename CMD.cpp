/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "GPU.h"
#include "CMD.h"
#include "CONFIG.h"
#include "CONSOLE.h"
#include "FDD.h"
#include "SD.h"
#include "RAM.h"
#include "i8080.h"
#include "BIOS.h"
#include "IPL.h"
#include "MEM.h"

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

	if (CONSOLE::mon_buffer[0] == 'B') {
		CMD::transfer();
		return;
	}

	GPU::color(1);
	GPU::print(F("INVALID COMMAND! {"));
	GPU::print(CONSOLE::mon_buffer);
	GPU::println("}");
	GPU::color(9);
}

void CMD::BIOS() {
	BIOS::BOOT();
}

void CMD::MEM_TEST() {
	GPU::color(2);
	CONSOLE::savecur();
	CONSOLE::xy(0, 0);
	RAM::MEM_TEST();
	CONSOLE::loadcur();
	GPU::color(9);
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
		case 'A' ... char(uint8_t('A') + FDD_NUM - 1) : driveno = (uint8_t(CONSOLE::mon_buffer[1]) - uint8_t('A'));
			break;
	}

	if (driveno == 0xFF) {
		GPU::color(1);
		GPU::println(F("Invalid disk!"));
		GPU::color(9);
		return;
	}

	GPU::println(F("Format disk"));
	GPU::println();

	//format
	start = FDD::SD_FDD_OFFSET[driveno];
	for (uint32_t i = 0; i < SD_BLK_SIZE; i++) {
		SD::_dsk_buffer[i] = CPM_EMPTY;
	}

	for (uint32_t i = 0; i < DISK_SIZE * TRACK_SIZE; i++) {
		GPU::print("\r");
		GPU::print(F("SECTOR "));
		GPU::print(String(i, DEC));
		res = SD::sd.writeBlock(i + start, SD::_dsk_buffer);
	}

	GPU::ok();
}

void CMD::transfer() {

	if (!CONSOLE::hexcheck(1, 4)) {
		GPU::error("HEX CHECK");
		return;
	}

	boolean _EOF = false;

	uint32_t adr = CONSOLE::kbd2word(1);
	uint16_t tmp_word = MEM::_AB;

	GPU::warning(F("Ready to receive file..."));
	//Serial.print(F("Address: "));
	//Serial.println(adr, HEX);

	uint16_t count = 0;
	do {

		CONSOLE::inChar = '\0';
		if (CONSOLE::con_ready()) {

			CONSOLE::inChar = CONSOLE::con_read();
			if (CONSOLE::inChar == CTRL_SLASH_KEY) {
				_EOF = true;
				continue;
			}

			MEM::_AB = adr;
			MEM::_DB = uint8_t(CONSOLE::inChar);
			MEM::_WR();
			adr++;
			count++;
			//Serial.print(CONSOLE::inChar);								//TODO: DEBUG
		}

	} while (!_EOF);

	GPU::warning("TRANSFER DONE");

	MEM::_AB = tmp_word;											//RETURN BACK
	MEM::_RD();

	GPU::print(String(count, DEC));
	GPU::println(F(" byte(s) were successfully received"));
}
