/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "CONFIG.h"
#include "CONSOLE.h"
#include "SD.h"
#include "MEM.h"

Sd2Card SD::sd;

uint8_t SD::_buffer[SD_BLK_SIZE];
uint8_t SD::_dsk_buffer[SD_BLK_SIZE];

void SD::init()
{
	CONSOLE::block("SD::init");

	uint32_t _cardsize;

	//SD card init
	do {
		SD::sd.init(SPI_FULL_SPEED, SS_SD_pin);
		_cardsize = SD::sd.cardSize();

		if (_cardsize != 0) {
			CONSOLE::ok();

			/*
			Serial.print(F("CARD SIZE: "));
			Serial.print(_cardsize);
			Serial.println(F(" SECTORS"));
			*/
		} else {
			delay(250);
		}

	} while (_cardsize == 0);


	CONSOLE::block("RAM::init");

	if (SD::eraseSD(SD_MEM_OFFSET, 65536UL / CACHE_LINE_SIZE * MMU_BANKS_NUM) == false) {
		CONSOLE::error();
	}

	uint8_t LRC;
	uint32_t blk;
	uint32_t blk_end;
	uint8_t res;

	blk = SD_MEM_OFFSET;
	blk_end = blk + (65536UL / CACHE_LINE_SIZE) * MMU_BANKS_NUM;

	do {
		LRC = 0;									//LRC reset
		for (uint32_t j = 0; j < CACHE_LINE_SIZE; j++) {
			SD::_buffer[j] = 0;
			LRC = 0 ^ LRC;							//LRC calculation
		}
		SD::_buffer[CACHE_LINE_SIZE] = LRC;			//LRC add
		res = SD::writeSD(blk);
		blk++;
	} while (blk < blk_end);

	CONSOLE::ok();
}

void SD::MEM_TEST() {

	CONSOLE::lnblockln("RAM TESTING");

	uint8_t CHECKED_BANKS;
	switch (MEM::RAM_TEST_MODE) {
	case 0:
		CHECKED_BANKS = 1;
		break;
	case 1:
		CHECKED_BANKS = MMU_BANKS_NUM;
		break;
	}

	for (uint8_t bank = 0; bank < CHECKED_BANKS; bank++) {

		CONSOLE::print(F("BANK "));
		Serial.print(bank, HEX);

		for (uint8_t block = 0; block < MMU_BLOCKS_NUM; block++) {
			MEM::bank_set(block, bank);
		}

		CONFIG::RAM_AVAIL = MEM::MEM_TEST(true);

		Serial.print(CONFIG::RAM_AVAIL / 1024, DEC);
		CONSOLE::println("K");

		if (CONFIG::RAM_AVAIL != 0x10000) {
			CONSOLE::error();
		}
	}

	//ACTIVATE BANK 0
	for (uint8_t block = 0; block < MMU_BLOCKS_NUM; block++) {
		MEM::bank_set(block, 0);
	}

	Serial.print(CONFIG::RAM_AVAIL, DEC);
	CONSOLE::print(" X ");
	Serial.print(MMU_BANKS_NUM, DEC);
	CONSOLE::blockln(F(" BYTE(S) OF RAM ARE AVAILABLE"));

	CONSOLE::block("RAM TESTING");
	CONSOLE::ok();
}

//block read from SD
uint8_t SD::readSD(uint32_t blk, uint16_t offset) {
	uint8_t res;
	res = SD::sd.readBlock(blk, SD::_buffer, offset);
	return res;
}

//block write to SD
uint8_t SD::writeSD(uint32_t blk) {
	uint8_t res;
	res = SD::sd.writeBlock(blk, SD::_buffer);
	return res;
}

//erase SD
uint8_t SD::eraseSD(uint32_t blk, uint32_t len) {
	uint8_t res;
	res = SD::sd.erase(blk, blk + len - 1);
	return res;
}
