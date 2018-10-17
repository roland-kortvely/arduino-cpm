/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#ifndef _BIOS_h
#define _BIOS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class BIOS
{
 public:
	static bool IPL();
	static bool INT();

	static void _GOCPM(boolean jmp);
	static void _BOOT();
	static void _WBOOT();

	static void _BIOS_BOOT();
	static void _BIOS_WBOOT();
	static void _BIOS_CONST();
	static void _BIOS_CONIN();
	static void _BIOS_CONOUT();
	static void _BIOS_LIST();
	static void _BIOS_PUNCH();
	static void _BIOS_READER();
	static void _BIOS_LISTST();
	static void _BIOS_SETTRK();
	static void _BIOS_HOME();
	static void _BIOS_SELDSK();
	static void _BIOS_SETSEC();
	static void _BIOS_SETDMA();
	static void _BIOS_READ();
	static void _BIOS_WRITE();
	static void _BIOS_SECTRAN();
	static void _BIOS_RET();
};

#endif

