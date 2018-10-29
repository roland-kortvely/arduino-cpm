/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */

#include "GPU.h"
#include "CONFIG.h"
#include "MEM.h"
#include "i8080.h"
#include "i8080_helper.h"
#include "SD.h"
#include "FDD.h"
#include "BIOS.h"
#include "IO.h"
#include "IPL.h"
#include "CONSOLE.h"

void BIOS::BOOT()
{
	GPU::clrscr();							//clear screen

	while (!BIOS::IPL()) {						//initial loader
		delay(1000);
	}

	CONFIG::BIOS_INT = true;				//BIOS intercept enabled
	CONSOLE::MON = false;

	BIOS::call(_BIOS);						//JMP TO BIOS

	CONSOLE::MON = true;
}

bool BIOS::IPL()
{
	GPU::println("IPL PRECHECK");

	uint8_t checksum = 0x00;
	uint8_t res;
	uint8_t d8;

	char hex[2];

	/*
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
	*/
	//GPU::println("");
	//GPU::println(F("IPL"));

	I8080::_SP = SP_INIT;

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

	GPU::println("");
	GPU::print(F("Checksum: "));
	sprintf(hex, "%02X", checksum);
	GPU::print(hex);
	GPU::print(" ");

	if (checksum != CPMSYS_CS) {

		GPU::error(F("\nCPM.SYS CHECKSUM ERROR!"));
		return false;
	}

	GPU::println(F("O.K.!"));

	for (uint16_t j = CPM_LBL_START; j < (CPM_LBL_START + CPM_LBL_LEN); j++) {
		MEM::_AB = CBASE + j;
		MEM::_RD();
		GPU::write(MEM::_DB);
	}

	GPU::println("");
	GPU::print(F("Serial: "));
	for (uint16_t j = CPM_SERIAL_START; j < (CPM_SERIAL_START + CPM_SERIAL_LEN); j++) {
		MEM::_AB = CBASE + j;
		MEM::_RD();
		sprintf(hex, "%02X", MEM::_DB);
		GPU::print(hex);
	}
	GPU::println("");

	uint16_t i = _DPBASE;

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

	return true;
}

void BIOS::call(word addr)
{

	byte cmd;
	bool exe_flag;

	CONFIG::exitFlag = false;
	I8080::_PC = addr;

	do
	{
		MEM::_AB = I8080::_PC;

		if (CONFIG::exitFlag) { 
			break; 
		}							

		if (BIOS::INT()) {
			break;
		}

		MEM::_RD();													//(AB) -> INSTR  instruction fetch
		I8080::_IR = MEM::_DB;

		((EXEC)pgm_read_word(&MAP_ARR[I8080::_IR]))();				//decode

	} while (true);

	if (MEM::MEM_ERR) {
		MEM::MEM_ERR = false;
		GPU::clrscr();
		GPU::error("MEMORY ERROR!");
	}
}

bool BIOS::INT() {

	switch (I8080::_PC) {

	case _BIOS + 0U:						//BOOT
		BIOS::_BIOS_BOOT();
		break;

	case _BIOS + 3U:						//WBOOT
		BIOS::_BIOS_WBOOT();
		break;

		//ASCII 7 ‚ 0
		//CTRL-Z 0x1A
	case _BIOS + 6U:						//CONST
		BIOS::_BIOS_CONST();
		break;

	case _BIOS + 9U:						//CONIN
		BIOS::_BIOS_CONIN();
		break;

	case _BIOS + 0xcU:						//CONOUT
		BIOS::_BIOS_CONOUT();
		break;

	case _BIOS + 0xfU:						//LIST
		BIOS::_BIOS_LIST();
		break;

	case _BIOS + 0x12U:						//PUNCH
		BIOS::_BIOS_PUNCH();
		break;

	case _BIOS + 0x15U:						//READER 
		BIOS::_BIOS_READER();
		break;

	case _BIOS + 0x2dU:						//LISTST
		BIOS::_BIOS_LISTST();
		break;

	case _BIOS + 0x18U:						//HOME
		BIOS::_BIOS_HOME();
		break;

	case _BIOS + 0x1bU:						//SELDSK
		BIOS::_BIOS_SELDSK();
		break;

	case _BIOS + 0x1eU:						//SETTRK
		BIOS::_BIOS_SETTRK();
		break;

	case _BIOS + 0x21U:						//SETSEC
		BIOS::_BIOS_SETSEC();
		break;

	case _BIOS + 0x24U:						//SETDMA
		BIOS::_BIOS_SETDMA();
		break;

	case _BIOS + 0x27U:						//READ
		BIOS::_BIOS_READ();
		break;

	case _BIOS + 0x2aU:						//WRITE
		BIOS::_BIOS_WRITE();
		break;

	case _BIOS + 0x30U:						//SECTRAN
		BIOS::_BIOS_SECTRAN();
		break;
		/*
	default:
		CONFIG::exitFlag = true;			//BIOS error
		return true;
		Serial.println("!BIOS::IPL");
		break;
		*/
	}

	return false;
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

	GPU::color(2);
	GPU::lnblockln(F("BIOS {BOOT}"));
	GPU::color(9);
	delay(1000);

	MEM::_AB = IOBYTE;
	MEM::_DB = 0x00;
	MEM::_WR();					//IOBYTE clear
	MEM::_AB = CDISK;
	MEM::_DB = 0x00;
	MEM::_WR();					//select disk 0

	//INITIALIZE AND GO TO CP/M
	_GOCPM(true);
}

void BIOS::_WBOOT() {

	GPU::color(2);
	GPU::lnblockln("BIOS {WBOOT}");
	GPU::color(9);
	delay(1000);

	//BIOS IPL
	I8080::_SP = 0x80;

	while (!BIOS::IPL()) {						//initial loader
		Serial.println("!BIOS::WBOT::IPL");
		delay(1000);
	}

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
	_rC = 0;											//track 0
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
