/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "CONFIG.h"
#include "MEM.h"
#include "i8080.h"
#include "SD.h"
#include "FDD.h"
#include "BIOS.h"
#include "IO.h"
#include "CONSOLE.h"

bool BIOS::IPL()
{

	Serial.println(F("IPL PRECHECK"));

	uint16_t i;

	uint8_t checksum = 0x00;
	uint8_t res;
	uint8_t d8;

	boolean success = false;

	char hex[2];

	Serial.print(RAM_SIZE, DEC);
	Serial.println("K SYSTEM");
	Serial.print("CBASE: ");
	Serial.write(0x09);
	Serial.println(CBASE, HEX);
	Serial.print("FBASE: ");
	Serial.write(0x09);
	Serial.println(FBASE, HEX);
	Serial.print("BIOS: ");
	Serial.write(0x09);
	Serial.print(_BIOS_LO, HEX);
	Serial.print(" ... ");
	Serial.println(_BIOS_HI, HEX);
	Serial.println("");
	Serial.println(F("IPL"));

	I8080::I8080::_SP = SP_INIT;
	FDD::FDD_REG_DRV = 0;
	FDD::FDD_REG_TRK = 0;
	FDD::FDD_REG_SEC = 1;

	//reading from SD
	for (uint16_t j = 0; j < CPMSYS_COUNT; j++) {
		for (uint16_t k = 0; k < 512; k += SD_BLK_SIZE) {

			res = SD::readSD(j + CPMSYS_START, k);

			for (uint16_t i = 0; i < SD_BLK_SIZE; i++) {
				SD::_dsk_buffer[i] = SD::_buffer[i];
			}
			for (uint16_t i = 0; i < SD_BLK_SIZE; i++) {
				MEM::_AB = CBASE + i + k + j * 512;
				MEM::_DB = SD::_dsk_buffer[i];
				MEM::_WR();
			}
		}

		IO::_setPORT(SIOA_CON_PORT_DATA, '.');
	}

	//checksum checking
	for (uint16_t j = 0; j < CPMSYS_COUNT; j++) {
		for (uint16_t i = 0; i < 512; i++) {
			MEM::_AB = CBASE + i + j * 512;
			MEM::_RD();
			d8 = MEM::_DB;
			checksum = checksum + d8;
		}
	}

	Serial.println("");
	Serial.print(F("Checksum: "));
	sprintf(hex, "%02X", checksum);
	Serial.print(hex);
	Serial.print(" ");

	if (checksum != CPMSYS_CS) {

		CONSOLE::error(F("\nCPM.SYS CHECKSUM ERROR!"));
		success = false;

		return success;
	}
	else {
		Serial.println(F("O.K.!"));
		success = true;

		for (uint16_t j = CPM_LBL_START; j < (CPM_LBL_START + CPM_LBL_LEN); j++) {
			MEM::_AB = CBASE + j;
			MEM::_RD();
			Serial.write(MEM::_DB);
		}

		Serial.println("");
		Serial.print(F("Serial: "));
		for (uint16_t j = CPM_SERIAL_START; j < (CPM_SERIAL_START + CPM_SERIAL_LEN); j++) {
			MEM::_AB = CBASE + j;
			MEM::_RD();
			sprintf(hex, "%02X", MEM::_DB);
			Serial.print(hex);
		}
		Serial.println("");


		i = _DPBASE;

		for (uint16_t l = 0; l < FDD_NUM; l++) {
			MEM::_AB = i;
			MEM::_DB = 0x00;
			MEM::_WR();
			i++;
			MEM::_AB = i;
			MEM::_DB = 0x00;
			MEM::_WR();
			i++;
			MEM::_AB = i;
			MEM::_DB = 0x00;
			MEM::_WR();
			i++;
			MEM::_AB = i;
			MEM::_DB = 0x00;
			MEM::_WR();
			i++;
			MEM::_AB = i;
			MEM::_DB = 0x00;
			MEM::_WR();
			i++;
			MEM::_AB = i;
			MEM::_DB = 0x00;
			MEM::_WR();
			i++;
			MEM::_AB = i;
			MEM::_DB = 0x00;
			MEM::_WR();
			i++;
			MEM::_AB = i;
			MEM::_DB = 0x00;
			MEM::_WR();
			//DIRBUF
			i++;
			MEM::_AB = i;
			MEM::_DB = lowByte(_DIRBUF);
			MEM::_WR();
			i++;
			MEM::_AB = i;
			MEM::_DB = highByte(_DIRBUF);
			MEM::_WR();
			//DPB
			i++;
			MEM::_AB = i;
			MEM::_DB = lowByte(_DPBLK);
			MEM::_WR();
			i++;
			MEM::_AB = i;
			MEM::_DB = highByte(_DPBLK);
			MEM::_WR();
			//CSV
			i++;
			MEM::_AB = i;
			MEM::_DB = lowByte(_CHK00);
			MEM::_WR();
			i++;
			MEM::_AB = i;
			MEM::_DB = highByte(_CHK00);
			MEM::_WR();
			//ALV
			i++;
			MEM::_AB = i;
			MEM::_DB = lowByte(_ALL00);
			MEM::_WR();
			i++;
			MEM::_AB = i;
			MEM::_DB = highByte(_ALL00);
			MEM::_WR();
			i++;
		}

		//DPB init
		i = _DPBLK;
		//SPT
		MEM::_AB = i;
		MEM::_DB = 26;
		MEM::_WR();
		MEM::_AB = i + 1;
		MEM::_DB = 0;
		MEM::_WR();
		//BSH
		MEM::_AB = i + 2;
		MEM::_DB = 3;
		MEM::_WR();
		//BLM
		MEM::_AB = i + 3;
		MEM::_DB = 7;
		MEM::_WR();
		//EXM
		MEM::_AB = i + 4;
		MEM::_DB = 0;
		MEM::_WR();
		//DSM
		MEM::_AB = i + 5;
		MEM::_DB = 242;
		MEM::_WR();
		MEM::_AB = i + 6;
		MEM::_DB = 0;
		MEM::_WR();
		//DRM
		MEM::_AB = i + 7;
		MEM::_DB = 63;
		MEM::_WR();
		MEM::_AB = i + 8;
		MEM::_DB = 0;
		MEM::_WR();
		//AL0    
		MEM::_AB = i + 9;
		MEM::_DB = 192;
		MEM::_WR();
		//AL1
		MEM::_AB = i + 0xA;
		MEM::_DB = 0;
		MEM::_WR();
		//CKS
		MEM::_AB = i + 0xB;
		MEM::_DB = 0;
		MEM::_WR();
		MEM::_AB = i + 0xC;
		MEM::_DB = 0;
		MEM::_WR();
		//OFF
		MEM::_AB = i + 0xD;
		MEM::_DB = 0;
		MEM::_WR();
		MEM::_AB = i + 0xE;
		MEM::_DB = 0;
		MEM::_WR();
		MEM::_AB = i + 0xF;
		MEM::_DB = 0;
		MEM::_WR();

		/*
		   BLS       BSH     BLM           EXM
		 -----      ---     ---     DSM<256   DSM>=256
		  1024       3       7         0        n/a
		 */
	}

	return success;
}

bool BIOS::INT() {

	bool exe_flag = true;

	/*
	if (I8080::I8080::_PC == FBASE) {
		if (DEBUG)
		{
			color(3);
			Serial.print(F("BDOS Fn:"));
			Serial.print(_Regs[_Reg_C], HEX);
			Serial.print(F(" DE="));
			Serial.print(_Regs[_Reg_D], HEX);
			Serial.println(_Regs[_Reg_E], HEX);
			color(9);
		}
	}*/

	if ((I8080::I8080::_PC >= _BIOS_LO) && (I8080::I8080::_PC < _BIOS_HI) && CONFIG::BIOS_INT) {
		switch (I8080::I8080::_PC) {
		case _BIOS + 0U://BOOT
			BIOS::_BIOS_BOOT();
			exe_flag = false;
			break;
		case _BIOS + 3U://WBOOT
			BIOS::_BIOS_WBOOT();
			exe_flag = false;
			break;
			//ASCII 7 ‚ 0
			//CTRL-Z 0x1A
		case _BIOS + 6U://CONST
			BIOS::_BIOS_CONST();
			exe_flag = false;
			break;
		case _BIOS + 9U://CONIN
			BIOS::_BIOS_CONIN();
			exe_flag = false;
			break;
		case _BIOS + 0xcU://CONOUT
			BIOS::_BIOS_CONOUT();
			exe_flag = false;
			break;
		case _BIOS + 0xfU://LIST
			BIOS::_BIOS_LIST();
			exe_flag = false;
			break;
		case _BIOS + 0x12U://PUNCH
			BIOS::_BIOS_PUNCH();
			exe_flag = false;
			break;
		case _BIOS + 0x15U://READER 
			BIOS::_BIOS_READER();
			exe_flag = false;
			break;
		case _BIOS + 0x2dU://LISTST
			BIOS::_BIOS_LISTST();
			exe_flag = false;
			break;
		case _BIOS + 0x18U://HOME
			BIOS::_BIOS_HOME();
			exe_flag = false;
			break;
		case _BIOS + 0x1bU://SELDSK
			BIOS::_BIOS_SELDSK();
			exe_flag = false;
			break;
		case _BIOS + 0x1eU://SETTRK
			BIOS::_BIOS_SETTRK();
			exe_flag = false;
			break;
		case _BIOS + 0x21U://SETSEC
			BIOS::_BIOS_SETSEC();
			exe_flag = false;
			break;
		case _BIOS + 0x24U://SETDMA
			BIOS::_BIOS_SETDMA();
			exe_flag = false;
			break;
		case _BIOS + 0x27U://READ
			BIOS::_BIOS_READ();
			exe_flag = false;
			break;
		case _BIOS + 0x2aU://WRITE
			BIOS::_BIOS_WRITE();
			exe_flag = false;
			break;
		case _BIOS + 0x30U://SECTRAN
			BIOS::_BIOS_SECTRAN();
			exe_flag = false;
			break;
		default:
			exe_flag = false;
			CONFIG::exitFlag = true;	//BIOS error
			break;
		}

		return exe_flag;
	}
}

void BIOS::_GOCPM(boolean jmp) {
	//JMP TO WBOOT
	MEM::_AB = JMP_BOOT;
	MEM::_DB = 0xC3;
	MEM::_WR();
	MEM::_AB = JMP_BOOT + 1;
	MEM::_DB = lowByte(_BIOS + 3);
	MEM::_WR();
	MEM::_AB = JMP_BOOT + 2;
	MEM::_DB = highByte(_BIOS + 3);
	MEM::_WR();
	//JMP TO BDOS
	MEM::_AB = JMP_BDOS;
	MEM::_DB = 0xC3;
	MEM::_WR();
	MEM::_AB = JMP_BDOS + 1;
	MEM::_DB = lowByte(FBASE);
	MEM::_WR();
	MEM::_AB = JMP_BDOS + 2;
	MEM::_DB = highByte(FBASE);
	MEM::_WR();
	//SETDMA 0x80
	MEM::_AB = word(FDD_PORT_DMA_ADDR_LO, FDD_PORT_DMA_ADDR_LO);
	MEM::_DB = 0x80;
	IO::_OUTPORT();
	MEM::_AB = word(FDD_PORT_DMA_ADDR_HI, FDD_PORT_DMA_ADDR_HI);
	MEM::_DB = 0x00;
	IO::_OUTPORT();
	//GET CURRENT DISK NUMBER   SEND TO THE CCP
	MEM::_AB = CDISK;
	MEM::_RD();
	_rC = MEM::_DB;

	//GO TO CP/M FOR FURTHER PROCESSING
	if (jmp) {
		I8080::_PC = CBASE;
		MEM::_AB = I8080::_PC;
	}
}

void BIOS::_BOOT() {
	//message BOOT

	Serial.println("");
	Serial.println(F("BOOT"));

	MEM::_AB = IOBYTE;
	MEM::_DB = 0x00;
	MEM::_WR();//IOBYTE clear
	MEM::_AB = CDISK;
	MEM::_DB = 0x00;
	MEM::_WR();//select disk 0
	//INITIALIZE AND GO TO CP/M
	_GOCPM(true);
}

void BIOS::_WBOOT() {
	boolean load;
	//message WBOOT
	Serial.println("");
	Serial.println(F("WBOOT"));
	//USE SPACE BELOW BUFFER FOR STACK
	I8080::_SP = 0x80;
	do {
		load = BIOS::IPL();
	} while (!load);
	//INITIALIZE AND GO TO CP/M
	_GOCPM(true);
}

void BIOS::_BIOS_BOOT() {
	_BOOT();
}


void BIOS::_BIOS_WBOOT() {
	_WBOOT();
}


void BIOS::_BIOS_CONST() {
	MEM::_AB = word(SIOA_CON_PORT_STATUS, SIOA_CON_PORT_STATUS);
	IO::_INPORT();
	if ((MEM::_DB & 0x20) != 0) {
		_rA = 0xFF;
	}
	else {
		_rA = 0x00;
	}
	BIOS::_BIOS_RET();
}


void BIOS::_BIOS_CONIN() {
	MEM::_AB = word(SIOA_CON_PORT_DATA, SIOA_CON_PORT_DATA);
	IO::_INPORT();
	_rA = MEM::_DB & B01111111;
	BIOS::_BIOS_RET();
}


void BIOS::_BIOS_CONOUT() {
	MEM::_AB = SIOA_CON_PORT_DATA;
	MEM::_DB = _rC;
	IO::_OUTPORT();
	BIOS::_BIOS_RET();
}

void BIOS::_BIOS_LIST() {
	BIOS::_BIOS_RET();
}

void BIOS::_BIOS_PUNCH() {
	BIOS::_BIOS_RET();
}

void BIOS::_BIOS_READER() {
	MEM::_AB = word(SIOA_CON_PORT_DATA, SIOA_CON_PORT_DATA);
	IO::_INPORT();
	_rA = MEM::_DB & B01111111;
	//ACK sent
	MEM::_AB = word(SIOA_CON_PORT_DATA, SIOA_CON_PORT_DATA);
	MEM::_DB = ACK;
	IO::_OUTPORT();
	BIOS::_BIOS_RET();
}

void BIOS::_BIOS_LISTST() {

	BIOS::_BIOS_RET();
}

void BIOS::_BIOS_SETTRK() {
	MEM::_AB = word(FDD_PORT_TRK, FDD_PORT_TRK);
	MEM::_DB = _rC;
	IO::_OUTPORT();
	BIOS::_BIOS_RET();
}

void BIOS::_BIOS_HOME() {
	_rC = 0;//track 0
	MEM::_AB = word(FDD_PORT_TRK, FDD_PORT_TRK);
	MEM::_DB = _rC;
	IO::_OUTPORT();
	BIOS::_BIOS_RET();
}

void BIOS::_BIOS_SELDSK() {
	uint16_t a16;
	uint8_t d8;
	d8 = _rC;
	if (d8 > (FDD_NUM - 1)) {
		_rH = 0;
		_rL = 0;
	}
	else {
		MEM::_AB = word(FDD_PORT_DRV, FDD_PORT_DRV);
		MEM::_DB = d8;
		IO::_OUTPORT();
		a16 = d8 * 16;
		a16 = a16 + _DPBASE;
		_rH = highByte(a16);
		_rL = lowByte(a16);
	}
	BIOS::_BIOS_RET();
}

void BIOS::_BIOS_SETSEC() {
	MEM::_AB = word(FDD_PORT_SEC, FDD_PORT_SEC);
	MEM::_DB = _rC;
	IO::_OUTPORT();
	BIOS::_BIOS_RET();
}

void BIOS::_BIOS_SETDMA() {
	MEM::_AB = word(FDD_PORT_DMA_ADDR_LO, FDD_PORT_DMA_ADDR_LO);
	MEM::_DB = _rC;
	IO::_OUTPORT();
	MEM::_AB = word(FDD_PORT_DMA_ADDR_HI, FDD_PORT_DMA_ADDR_HI);
	MEM::_DB = _rB;
	IO::_OUTPORT();
	BIOS::_BIOS_RET();
}

void BIOS::_BIOS_READ() {
	MEM::_AB = word(FDD_PORT_CMD, FDD_PORT_CMD);
	MEM::_DB = FDD_RD_CMD;
	IO::_OUTPORT();
	MEM::_AB = word(FDD_PORT_CMD, FDD_PORT_CMD);
	IO::_INPORT();
	if (MEM::_DB) {
		_rA = DISK_SUCCESS;
	}
	else {
		_rA = DISK_ERROR;
	}
	BIOS::_BIOS_RET();
}

void BIOS::_BIOS_WRITE() {
	MEM::_AB = word(FDD_PORT_CMD, FDD_PORT_CMD);
	MEM::_DB = FDD_WRT_CMD;
	IO::_OUTPORT();
	MEM::_AB = word(FDD_PORT_CMD, FDD_PORT_CMD);
	IO::_INPORT();
	if (MEM::_DB) {
		_rA = DISK_SUCCESS;
	}
	else {
		_rA = DISK_ERROR;
	}
	BIOS::_BIOS_RET();
}

void BIOS::_BIOS_SECTRAN() {
	//_Regs[_Reg_C] -> logical sector (from 0)
	_rL = _rC + 1;
	_rH = 0;
	BIOS::_BIOS_RET();
}

void BIOS::_BIOS_RET() {
	uint16_t a16;
	//RET
	MEM::_AB = I8080::_SP;
	MEM::_RD();
	a16 = MEM::_DB;
	I8080::_SP++;
	MEM::_AB = I8080::_SP;
	MEM::_RD();
	a16 = a16 + 256 * MEM::_DB;
	I8080::_SP++;
	I8080::_PC = a16;
	MEM::_AB = I8080::_PC;
}
