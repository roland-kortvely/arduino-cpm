/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "EEPROM.h"
#include "CONFIG.h"
#include "CONSOLE.h"
#include "MEM.h"
#include "SD.h"

volatile uint8_t  MEM::_DB;			//data bus buffer
volatile uint16_t MEM::_AB;			//data bus buffer

int MEM::EEPROM_SIZE = 256;
int MEM::EEPROM_DRIVES = 0x00;
int MEM::EEPROM_SENSE_SW = MEM::EEPROM_DRIVES + FDD_NUM;

boolean MEM::MEM_ERR = false;		//LRC memory error flag

uint8_t MEM::RAM_TEST_MODE = 1;		//memory check mpde

//CACHE
uint8_t  MEM::cache[CACHE_SIZE];
uint32_t MEM::cache_tag[CACHE_LINES_NUM];
uint16_t MEM::cache_start[CACHE_LINES_NUM];
boolean  MEM::cache_dirty[CACHE_LINES_NUM];

//MMU
uint8_t MEM::MMU_BLOCK_SEL_REG = 0x00;
uint8_t MEM::MMU_BANK_SEL_REG = 0x00;
uint8_t MEM::MMU_MAP[MMU_BLOCKS_NUM];

void MEM::init()
{
	//EEPROM init
	CONSOLE::block("EEPROM::init");

	if ((EEPROM.read(0xFE) != 0x55) || (EEPROM.read(0xFF) != 0xAA)) {

		//EEPROM clearing
		for (int i = 0; i < MEM::EEPROM_SIZE; i++) {
			EEPROM.write(i, 0);
		}

		//settings init
		for (int i = MEM::EEPROM_DRIVES; i < (MEM::EEPROM_DRIVES + FDD_NUM); i++) {
			EEPROM.write(i, uint8_t(i - MEM::EEPROM_DRIVES));
		}

		//sense switches off
		EEPROM.write(EEPROM_SENSE_SW, 0x00);

		//write signature to EEPROM
		//0xFE - 0x55
		EEPROM.write(0xFE, 0x55);

		//0xFF - 0xAA
		EEPROM.write(0xFF, 0xAA);

		CONSOLE::ok();
	}
	else {
		CONSOLE::skip();
	}

	//MMU init
	CONSOLE::block("MMU::init");

	for (uint32_t i = 0; i < MMU_BLOCKS_NUM; i++) {
		MEM::MMU_MAP[i] = 0;
	}
	MEM::MMU_BLOCK_SEL_REG = 0;

	//cache init
	for (uint32_t i = 0; i < CACHE_LINES_NUM; i++) {
		MEM::cache_tag[i] = 0xFFFFFFFF;
	}
	for (uint32_t i = 0; i < CACHE_LINES_NUM; i++) {
		MEM::cache_dirty[i] = true;
	}
	for (uint32_t i = 0; i < CACHE_LINES_NUM; i++) {
		MEM::cache_start[i] = i * CACHE_LINE_SIZE;
	}
	CONSOLE::ok();
}

void MEM::bank_set(uint8_t block, uint8_t bank) {
	MEM::MMU_MAP[block] = bank;
}

uint8_t MEM::bank_get(uint8_t block) {
	return MEM::MMU_MAP[block];
}

//address <- _AB
//data -> _DB
void MEM::_RD() {

	uint32_t blk;
	uint32_t blk_tmp;
	uint16_t start_tmp;
	uint16_t i;
	uint8_t sel_blk;
	uint8_t res;
	uint8_t LRC;

	if (_AB > MEM_MAX) {
		_DB = 0xFF;//not memory
		return;
	}

	blk = ((uint32_t)(_AB)+(uint32_t)((MMU_MAP[_AB / MMU_BLOCK_SIZE]) * 65536UL)) / CACHE_LINE_SIZE;
	blk = blk + SD_MEM_OFFSET;
	//Serial.println(blk, HEX);
	sel_blk = 0xff;

	i = 0;
	do {
		if (blk == cache_tag[i]) {
			sel_blk = i;
		}
		i++;
	} while ((sel_blk == 0xff) && (i < CACHE_LINES_NUM));

	if (sel_blk == 0xff) { //cache miss
		sel_blk = CACHE_LINES_NUM - 1;
		if (cache_tag[sel_blk] != CACHE_LINE_EMPTY)
		{
			if (cache_tag[0] != CACHE_LINE_EMPTY) {
				//line 0 -> SD
				if (cache_dirty[0]) {
					LRC = 0;//LRC reset
					for (i = 0; i < CACHE_LINE_SIZE; i++) {
						SD::_buffer[i] = cache[cache_start[0] + i];
						LRC = SD::_buffer[i] ^ LRC;					//LRC calculation
					}
					SD::_buffer[CACHE_LINE_SIZE] = LRC;				//LRC write
					res = SD::writeSD(cache_tag[0]);
				}
			}
			//move up
			start_tmp = cache_start[0];
			blk_tmp = cache_tag[0];
			for (i = 1; i < CACHE_LINES_NUM; i++) {
				cache_start[i - 1] = cache_start[i];
				cache_tag[i - 1] = cache_tag[i];
			}
			cache_start[CACHE_LINES_NUM - 1] = start_tmp;
			cache_tag[CACHE_LINES_NUM - 1] = blk_tmp;
		}
		//read new line from SD
		cache_tag[sel_blk] = blk;
		res = SD::readSD(blk, 0);
		LRC = 0;//LRC reset
		for (i = 0; i < CACHE_LINE_SIZE; i++) {
			cache[cache_start[sel_blk] + i] = SD::_buffer[i];
			LRC = SD::_buffer[i] ^ LRC;//LRC calculation
		}
		if (SD::_buffer[CACHE_LINE_SIZE] != LRC) {
			MEM_ERR = true;
			CONFIG::exitFlag = true;//quit to BIOS
			_DB = 0x00;
			return;
		}
		cache_dirty[sel_blk] = false;
	}
	else { //cache hit
		if (sel_blk != CACHE_LINES_NUM - 1) {
			//cache lines swap
			start_tmp = cache_start[sel_blk + 1];
			cache_start[sel_blk + 1] = cache_start[sel_blk];
			cache_start[sel_blk] = start_tmp;
			blk_tmp = cache_tag[sel_blk + 1];
			cache_tag[sel_blk + 1] = cache_tag[sel_blk];
			cache_tag[sel_blk] = blk_tmp;
			sel_blk++;
		}
	}
	_DB = cache[cache_start[sel_blk] + (_AB & (CACHE_LINE_SIZE - 1))];//read from cache
}

void MEM::_WR() {
	uint32_t blk;
	uint32_t blk_tmp;
	uint16_t start_tmp;
	uint16_t i;
	uint8_t sel_blk;
	uint8_t res;
	uint8_t LRC;
	if (_AB > MEM_MAX) {
		return;
	}
	blk = ((uint32_t)(_AB)+(uint32_t)((MMU_MAP[_AB / MMU_BLOCK_SIZE]) * 65536UL)) / CACHE_LINE_SIZE;
	blk = blk + SD_MEM_OFFSET;
	sel_blk = 0xff;
	i = 0;
	do {
		if (blk == cache_tag[i]) {
			sel_blk = i;
		}
		i++;
	} while ((sel_blk == 0xff) && (i < CACHE_LINES_NUM));
	if (sel_blk == 0xff) { //cache miss
		sel_blk = CACHE_LINES_NUM - 1;
		if (cache_tag[sel_blk] != CACHE_LINE_EMPTY)
		{
			if (cache_tag[0] != CACHE_LINE_EMPTY) {
				//line 0 -> SD
				if (cache_dirty[0]) {
					LRC = 0;												//LRC reset
					for (i = 0; i < CACHE_LINE_SIZE; i++) {
						SD::_buffer[i] = cache[cache_start[0] + i];
						LRC = SD::_buffer[i] ^ LRC;							//LRC calculation
					}
					SD::_buffer[CACHE_LINE_SIZE] = LRC;						//LRC add
					res = SD::writeSD(cache_tag[0]);
				}
			}
			//move up
			start_tmp = cache_start[0];
			blk_tmp = cache_tag[0];
			for (i = 1; i < CACHE_LINES_NUM; i++) {
				cache_start[i - 1] = cache_start[i];
				cache_tag[i - 1] = cache_tag[i];
			}
			cache_start[CACHE_LINES_NUM - 1] = start_tmp;
			cache_tag[CACHE_LINES_NUM - 1] = blk_tmp;
		}
		//read new line from SD
		cache_tag[sel_blk] = blk;
		res = SD::readSD(blk, 0);
		LRC = 0;															//LRC reset
		for (i = 0; i < CACHE_LINE_SIZE; i++) {
			cache[cache_start[sel_blk] + i] = SD::_buffer[i];
			LRC = SD::_buffer[i] ^ LRC;										//LRC calculation
		}
		if (SD::_buffer[CACHE_LINE_SIZE] != LRC) {
			MEM_ERR = true;
			CONFIG::exitFlag = true;										//quit to BIOS
			return;
		}
		cache_dirty[sel_blk] = false;
	}
	else { //cache hit
		if (sel_blk != CACHE_LINES_NUM - 1) {
			start_tmp = cache_start[sel_blk + 1];
			cache_start[sel_blk + 1] = cache_start[sel_blk];
			cache_start[sel_blk] = start_tmp;
			blk_tmp = cache_tag[sel_blk + 1];
			cache_tag[sel_blk + 1] = cache_tag[sel_blk];
			cache_tag[sel_blk] = blk_tmp;
			sel_blk++;
		}
	}
	cache[cache_start[sel_blk] + (_AB & (CACHE_LINE_SIZE - 1))] = _DB;		//cache update
	cache_dirty[sel_blk] = true;
}

uint8_t MEM::_get(uint16_t adr) {
	MEM::_AB = adr;
	MEM::_RD();
	return MEM::_DB;
}

void MEM::_set(uint16_t adr, uint8_t dat) {
	MEM::_AB = adr;
	MEM::_DB = dat;
	MEM::_WR();
}

uint32_t MEM::MEM_TEST(boolean brk) {

	uint32_t i;
	uint16_t j;
	uint32_t res;

	res = 0x10000L;

	//RAM write
	j = 0;
	for (i = 0; i <= 0xFFFF; i++) {
		MEM::_AB = i;
		MEM::_DB = pgm_read_byte_near(memtest_table + j);
		MEM::_WR();
		if ((i % 8192) == 0) {
			CONSOLE::print(".");
		}
		j++;
		if (j == MEMTEST_TABLE_SIZE) {
			j = 0;
		}
		if (brk && CONSOLE::con_ready()) {
			CONSOLE::con_read();
			return 0xFFFFF;						//break
		}
	}

	//RAM read
	j = 0;
	for (i = 0; i <= 0xFFFF; i++) {
		if ((i % 8192) == 0) {
			CONSOLE::print(".");
		}
		MEM::_AB = i;
		MEM::_RD();
		if (_DB != pgm_read_byte_near(memtest_table + j)) {
			if (res > i) {
				res = i;
			}
		}
		j++;
		if (j == MEMTEST_TABLE_SIZE) {
			j = 0;
		}
		if (brk && CONSOLE::con_ready()) {
			CONSOLE::con_read();
			return 0xFFFFF;//break
		}
	}

	//RAM write (inverse)
	j = 0;
	for (i = 0; i <= 0xFFFF; i++) {
		MEM::_AB = i;
		MEM::_DB = uint8_t(~(pgm_read_byte_near(memtest_table + j)));
		MEM::_WR();
		if ((i % 8192) == 0) {
			CONSOLE::print(".");
		}
		j++;
		if (j == MEMTEST_TABLE_SIZE) {
			j = 0;
		}
		if (brk && CONSOLE::con_ready()) {
			CONSOLE::con_read();
			return 0xFFFFF;//break
		}
	}

	//RAM read (inverse)
	j = 0;
	for (i = 0; i <= 0xFFFF; i++) {
		if ((i % 8192) == 0) {
			CONSOLE::print(".");
		}
		MEM::_AB = i;
		MEM::_RD();
		if (_DB != uint8_t(~(pgm_read_byte_near(memtest_table + j)))) {
			if (res > i) {
				res = i;
			}
		}
		j++;
		if (j == MEMTEST_TABLE_SIZE) {
			j = 0;
		}

		if (brk && CONSOLE::con_ready()) {
			CONSOLE::con_read();
			return 0xFFFFF;//break
		}
	}
	return res;
}
