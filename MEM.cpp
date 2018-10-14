/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "EEPROM.h"
#include "CONFIG.h"
#include "MEM.h"
#include "SD.h"

int MEM::EEPROM_SIZE = 256;
int MEM::EEPROM_DRIVES = 0x00;
int MEM::EEPROM_SENSE_SW = MEM::EEPROM_DRIVES + FDD_NUM;

boolean MEM::MEM_ERR = false;	//LRC memory error flag

uint8_t MEM::RAM_TEST_MODE;		//memory check mpde

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

	//EEPROM checking
	if ((EEPROM.read(0xFE) == 0x55) && (EEPROM.read(0xFF) == 0xAA)) {
		return;
	}

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

	//MMU init
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

	SD::init();
}
