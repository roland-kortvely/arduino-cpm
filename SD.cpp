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
