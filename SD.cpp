/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "SD.h"

void SD::init()
{
	//SD card init
	Serial.print(F("SD CARD INIT..."));
	do {
		card.init(SPI_FULL_SPEED, SS_SD_pin);
		_cardsize = card.cardSize();
		if (_cardsize != 0) {
			Serial.println(F("OK"));
			Serial.print(F("CARD SIZE: "));
			Serial.print(_cardsize);
			Serial.println(F(" SECTORS"));
		}
		else {
			delay(250);
		}
	} while (_cardsize == 0);
}
