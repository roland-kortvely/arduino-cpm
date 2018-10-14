/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "CONFIG.h"
#include "EEPROM.h"
#include "MEM.h"
#include "FDD.h"

uint32_t FDD::SD_FDD_OFFSET[FDD_NUM];

void FDD::init()
{
	//disks mount
	for (uint8_t k = 0; k < FDD_NUM; k++) {
		SD_FDD_OFFSET[k] = SD_DISKS_OFFSET + EEPROM.read(k) * SD_DISK_SIZE;
	}

	CONFIG::SENSE_SW = EEPROM.read(MEM::EEPROM_SENSE_SW);
}
