/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "Sd2Card.h"

#ifndef _SD_h
#define _SD_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class SD
{
public:

	static Sd2Card sd;
	static uint8_t _buffer[SD_BLK_SIZE];
	static uint8_t _dsk_buffer[SD_BLK_SIZE];

	static void init();

	static uint8_t readSD(uint32_t blk, uint16_t offset);
	static uint8_t writeSD(uint32_t blk);
	static uint8_t eraseSD(uint32_t blk, uint32_t len);
};

#endif
