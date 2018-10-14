/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#ifndef _MEM_h
#define _MEM_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif


class MEM
{
public:

	static int EEPROM_SIZE;
	static int EEPROM_DRIVES;
	static int EEPROM_SENSE_SW;

	static boolean MEM_ERR;
	static uint8_t RAM_TEST_MODE;

	static uint8_t  cache[CACHE_SIZE];					//cache
	static uint32_t cache_tag[CACHE_LINES_NUM];			//cache line tag (block #)
	static uint16_t cache_start[CACHE_LINES_NUM];		//cache line start
	static boolean  cache_dirty[CACHE_LINES_NUM];		//cache line dirty flag

	//MMU
	static uint8_t MMU_BLOCK_SEL_REG;					//block select register
	static uint8_t MMU_BANK_SEL_REG;					//bank register
	static uint8_t MMU_MAP[MMU_BLOCKS_NUM];//memory banking map

	static void init();
};

#endif
