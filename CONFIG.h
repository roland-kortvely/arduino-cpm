/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#ifndef _CONFIG_h
#define _CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

 //BOOT area
#define SEC_BUF 0x80 //sector buffer

constexpr auto SS_SD_pin = 10;								//SS pin (D10)
constexpr auto SD_BLK_SIZE = 128;							//SD block size

constexpr auto FDD_NUM = 4;
constexpr auto SD_DISKS_OFFSET = 0x0001000;
constexpr auto SD_DISK_SIZE = 0x0001000;
constexpr auto COM_BAUD = 9600;
constexpr auto CPM_EMPTY = 0xE5;							//empty byte (disk)
constexpr auto SECTOR_SIZE = 128;
constexpr auto TRACK_SIZE = 26;
constexpr auto DISK_SIZE = 77;
constexpr auto FDD_SIZE = TRACK_SIZE * DISK_SIZE;			//sectors
constexpr auto DISK_SUCCESS = 0;
constexpr auto DISK_ERROR = 1;

constexpr auto MEM_SIZE = 64;								//Sys RAM
constexpr auto MEM_MAX = (MEM_SIZE - 1) * 1024U + 1023U;	//max sys RAM addr
constexpr auto RAM_SIZE = 64;								//RAM Size

constexpr auto SD_MEM_OFFSET = 0x070000;					//memory offset in SD-card
constexpr auto MEMTEST_TABLE_SIZE = 33;

const static uint8_t PROGMEM memtest_table[] = {
  0x3D, 0x55, 0x5F, 0x15, 0x23, 0x47, 0x1C, 0x31, 0x48, 0x60, 0x35, 0x11, 0x4F, 0x2F, 0x2E, 0x14, 0x20, 0x5B, 0x39, 0x26, 0x09, 0x61, 0x34, 0x30, 0x50, 0x2B, 0x4B, 0x0F, 0x63, 0x1F, 0x10, 0x1E, 0x36,
};

//CACHE
constexpr auto CACHE_LINE_SIZE = 64;							//cache line size
constexpr auto CACHE_LINES_NUM = 8;								//cache lines number
constexpr auto CACHE_SIZE = CACHE_LINES_NUM * CACHE_LINE_SIZE;	//total cache size
constexpr auto CACHE_LINE_EMPTY = 0xFFFFFFFF;					//empty cache line flag

//MMU
//ports
constexpr auto MMU_BLOCK_SEL_PORT = 0xD0;						//block select port
constexpr auto MMU_BANK_SEL_PORT = 0xD1;						//bank select port

constexpr auto MMU_BANKS_NUM = 8;
constexpr auto MMU_BLOCK_SIZE = 4096;							//4096 bytes - block size 
constexpr auto MMU_BLOCKS_NUM = 65536UL / MMU_BLOCK_SIZE;		//blocks number

//mem constants
/*
 *	0000   - 00FF     System scratch area
 *	0100   - 33FF+b   TPA (Transient Program Area) - COM file area
 * 	3400+b - 3BFF+b   CCP - Console Command Processor
 *	3C00+b - 49FF+b   BDOS
 * 	4A00+b - 4FFF+b   CBIOS
 */
constexpr auto B_OFFSET = (RAM_SIZE - 20U) * 1024U;
constexpr auto BOOT = 0;
constexpr auto JMP_BOOT = 0;						//warm start jmp 0x4a03
constexpr auto IOBYTE = 3;							//INTEL I/O BYTE
constexpr auto CDISK = 4;							//CURRENT DISK NUMBER 0=A,...
constexpr auto LOGINBYTE = 4;
constexpr auto JMP_BDOS = 5;						//BDOS start jmp 0x3c06
constexpr auto TBASE = 0x100U;
constexpr auto CBASE = 0x3400U + B_OFFSET;			//CCP 0xDC00
constexpr auto FBASE = 0x3c06U + B_OFFSET;			//0xE406
constexpr auto SP_INIT = CBASE - 0x100U;
constexpr auto _BIOS = 0x4a00U + B_OFFSET;			//0xF200

constexpr auto _BIOS_LO = 0x4a00U + B_OFFSET;
constexpr auto _BIOS_HI = _BIOS_LO + 0x32U;
constexpr auto _DPBASE = _BIOS_HI + 1;				//DPB

constexpr auto  _DPBLK = _DPBASE + FDD_NUM * 16;	//DISK PARAMETER BLOCK, COMMON TO ALL DISKS

//SCRATCH RAM AREA FOR BDOS USE
constexpr auto _BEGDAT = _DPBLK + 16;	//BEGINNING OF DATA AREA
constexpr auto _DIRBUF = _BEGDAT;		//SCRATCH DIRECTORY AREA
constexpr auto _ALL00 = _DIRBUF + 128;	//ALLOCATION VECTOR 0
constexpr auto _ALL01 = _ALL00 + 31;	//ALLOCATION VECTOR 1
constexpr auto _ALL02 = _ALL01 + 31;	//ALLOCATION VECTOR 2
constexpr auto _ALL03 = _ALL02 + 31;	//ALLOCATION VECTOR 3
constexpr auto _CHK00 = _ALL03 + 31;	//CHECK VECTOR 0
constexpr auto _CHK01 = _CHK00 + 16;	//CHECK VECTOR 1
constexpr auto _CHK02 = _CHK01 + 16;	//CHECK VECTOR 2
constexpr auto _CHK03 = _CHK02 + 16;	//CHECK VECTOR 3
constexpr auto _ENDDAT = _CHK03 + 16;	//END OF DATA AREA

constexpr auto SENSE_SW_PORT = 0xFF;

constexpr auto MON_Y = 15;
constexpr auto CON_IN = 0;				// 0 - console | 1 - PS2
constexpr auto KBD_BUFFER_SIZE = 16;	//console input buffer size

class CONFIG
{
public:
	static uint8_t SENSE_SW;
	static boolean BIOS_INT; //BIOS Intercept flag
	static void init();
};

#endif
