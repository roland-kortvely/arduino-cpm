/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "CONFIG.h"
#include "IO.h"
#include "CONSOLE.h"
#include "MEM.h"
#include "FDD.h"
#include "SD.h"

volatile bool IO::exitFlag = false;

uint32_t IO::RAM_AVAIL = 0x10000L;

//AUX
bool    IO::LED_on = false;
uint8_t IO::LED_count;

//Other
uint8_t IO::SENSE_SW = 0x00;

void IO::init()
{

	CONSOLE::block("IO::init");

	pinMode(LED_pin, OUTPUT);
	digitalWrite(LED_pin, LOW);
	pinMode(IN_pin, INPUT);
	pinMode(OUT_pin, OUTPUT);
	digitalWrite(OUT_pin, LOW);

	CONSOLE::ok();
}

//address <- _AB
//data -> _DB
void IO::_INPORT() {
	uint8_t dat;
	boolean readyFlag = false;
	dat = 0x00;
	switch (lowByte(MEM::_AB)) {
		//SIO-A
	case SIOA_CON_PORT_STATUS:
		//bit 1 - ready to out (Altair)
		//bit 5 - ready to in (Altair)
		//bit 7 - ready to out (IMSAI)
		//bit 0 - ready to in (IMSAI)
		dat = 0x02 | 0x80;
		if (CONSOLE::con_ready()) {
			dat = dat | 0x20;
			dat = dat | 0x01;
		}
		break;
	case SIOA_CON_PORT_DATA:
		//input from console
		do {
			if (CONSOLE::con_ready()) {
				dat = uint8_t(CONSOLE::con_read());
				readyFlag = true;
			}
		} while (!readyFlag);
		break;
		//SIO-2
	case SIO2_CON_PORT_STATUS:
		//bit 1 - ready to out (Altair)
		//bit 0 - ready to in (Altair)
		dat = 0x02;
		if (CONSOLE::con_ready()) {
			dat = dat | 0x01;
		}
		break;
	case SIO2_CON_PORT_DATA:
		//input from console
		do {
			if (CONSOLE::con_ready()) {
				dat = uint8_t(CONSOLE::con_read());
				readyFlag = true;
			}
		} while (!readyFlag);
		break;
		//FDD ports
	case FDD_PORT_CMD:
		//status
		if (FDD::FDD_REG_STATUS) {
			dat = 1;
		}
		else {
			dat = 0;
		}
		break;
	case FDD_PORT_TRK:
		//track
		dat = FDD::FDD_REG_TRK;
		break;
	case FDD_PORT_SEC:
		//sector
		dat = FDD::FDD_REG_SEC;
		break;
	case FDD_PORT_DRV:
		//drive select
		dat = FDD::FDD_REG_DRV;
		break;
	case SENSE_SW_PORT:
		//Altair/IMSAI sense switch
		dat = SENSE_SW;
		break;
		//MMU registers
	case MMU_BLOCK_SEL_PORT:
		dat = MEM::MMU_BLOCK_SEL_REG;
		break;
	case MMU_BANK_SEL_PORT:
		dat = MEM::bank_get(MEM::MMU_BLOCK_SEL_REG);
		break;
	case IN_PORT:
		dat = digitalRead(IN_pin);
		if ((dat && 0x01) == 0x01) {
			dat = 0x01;
		}
		else {
			dat = 0x00;
		}
		break;
	}
	MEM::_DB = dat;
}

//address <- _AB
//data <- _DB
void IO::_OUTPORT() {
	uint8_t dat;
	uint16_t i;
	uint8_t res;
	uint32_t blk;
	dat = MEM::_DB;
	switch (lowByte(MEM::_AB)) {
		//console ports
		//SIO-A
	case SIOA_CON_PORT_DATA:
		//output to console
		Serial.write(dat);
		break;
		//SIO-2
	case SIO2_CON_PORT_DATA:
		//output to console
		Serial.write(dat);
		break;
		//FDD ports
	case FDD_PORT_CMD:
		//command
		if (dat == FDD_RD_CMD) {
			//sector read
			//blk = _getMEM(_FDD_SECTOR)-1;
			blk = FDD::FDD_REG_SEC - 1L;
			blk = blk + FDD::FDD_REG_TRK * TRACK_SIZE;
			blk = blk + FDD::SD_FDD_OFFSET[FDD::FDD_REG_DRV];
			res = SD::readSD(blk, 0);
			if (res == 1) {
				for (i = 0; i < SD_BLK_SIZE; i++) {
					SD::_dsk_buffer[i] = SD::_buffer[i];
				}
				for (i = 0; i < SD_BLK_SIZE; i++) {
					MEM::_set(FDD::FDD_REG_DMA + i, SD::_dsk_buffer[i]);
				}
				FDD::FDD_REG_STATUS = true;
			}
			else {
				FDD::FDD_REG_STATUS = false;
			}
		}
		if (dat == FDD_WRT_CMD) {
			//sector write
			blk = FDD::FDD_REG_SEC - 1L;
			blk = blk + FDD::FDD_REG_TRK * TRACK_SIZE;
			blk = blk + FDD::SD_FDD_OFFSET[FDD::FDD_REG_DRV];
			for (i = 0; i < SD_BLK_SIZE; i++) {
				SD::_dsk_buffer[i] = MEM::_get(FDD::FDD_REG_DMA + i);
			}
			for (i = 0; i < SD_BLK_SIZE; i++) {
				SD::_buffer[i] = SD::_dsk_buffer[i];
			}
			res = SD::writeSD(blk);
			if (res == 1) {
				FDD::FDD_REG_STATUS = true;
			}
			else {
				FDD::FDD_REG_STATUS = false;
			}
		}
		break;
	case FDD_PORT_TRK:
		//track
		FDD::FDD_REG_TRK = dat;
		break;
	case FDD_PORT_SEC:
		//sector
		FDD::FDD_REG_SEC = dat;
		break;
	case FDD_PORT_DRV:
		//drive select
		FDD::FDD_REG_DRV = dat;
		break;
	case FDD_PORT_DMA_ADDR_LO:
		FDD::FDD_REG_DMA = FDD::FDD_REG_DMA & 0xFF00;
		FDD::FDD_REG_DMA = FDD::FDD_REG_DMA | dat;
		break;
	case FDD_PORT_DMA_ADDR_HI:
		FDD::FDD_REG_DMA = FDD::FDD_REG_DMA & 0x00FF;
		FDD::FDD_REG_DMA = FDD::FDD_REG_DMA + dat * 256;
		break;
		//MMU registers
	case MMU_BLOCK_SEL_PORT:
		MEM::MMU_BLOCK_SEL_REG = dat;
		break;
	case MMU_BANK_SEL_PORT:
		MEM::bank_set(MEM::MMU_BLOCK_SEL_REG, dat);
		break;
	case OUT_PORT:
		//bit 0 out
		if ((dat && 0x01) == 0x01) {
			fastDigitalWrite(OUT_pin, HIGH);
		}
		else {
			fastDigitalWrite(OUT_pin, LOW);
		}
		break;
	}
}

uint8_t IO::_getPORT(uint16_t adr) {
	MEM::_AB = adr;
	IO::_INPORT();
	return MEM::_DB;
}

void IO::_setPORT(uint16_t adr, uint8_t dat) {
	MEM::_AB = adr;
	MEM::_DB = dat;
	IO::_OUTPORT();
}
