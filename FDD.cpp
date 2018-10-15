/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "CONFIG.h"
#include "EEPROM.h"
#include "CONSOLE.h"
#include "MEM.h"
#include "FDD.h"

uint32_t FDD::SD_FDD_OFFSET[FDD_NUM];

uint8_t  FDD::FDD_REG_SEC = 1;			//sector register
uint8_t  FDD::FDD_REG_TRK = 0;			//track register
uint8_t  FDD::FDD_REG_DRV = 0;			//drive register
boolean  FDD::FDD_REG_STATUS = false;	//true - O.K., false - ERROR
uint16_t FDD::FDD_REG_DMA = 0;			//DMA address register

void FDD::init()
{
	CONSOLE::block("FDD::init");

	//disks mount
	for (uint8_t k = 0; k < FDD_NUM; k++) {
		SD_FDD_OFFSET[k] = SD_DISKS_OFFSET + EEPROM.read(k) * SD_DISK_SIZE;
	}

	CONFIG::SENSE_SW = EEPROM.read(MEM::EEPROM_SENSE_SW);

	CONSOLE::ok();
}
