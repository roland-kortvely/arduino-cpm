/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "CONFIG.h"
#include "IPL.h"
#include "CONSOLE.h"
#include "IO.h"
#include "MEM.h"
#include "SD.h";
#include "FDD.h"
#include "RAM.h"
#include "i8080.h"
#include "i8080_helper.h"

void IPL::init()
{
	CONFIG::init();

	CONSOLE::lnblockln(F("INITIAL PROGRAM LOAD (IPL)"));

	IO::init();

	MEM::init();
	SD::init();
	FDD::init();

	RAM::init();

	CONSOLE::lnblockln("IPL DONE");
}

void IPL::call(word addr)
{
	byte cmd;
	bool exe_flag;

	CONFIG::exitFlag = false;
	I8080::_PC = addr;

	do
	{
		MEM::_AB = I8080::_PC;

		if (CONFIG::exitFlag) { break; }							//go to monitor

		if (BIOS::INT()) {
			break;
		}

		MEM::_RD();													//(AB) -> INSTR  instruction fetch
		I8080::_IR = MEM::_DB;

		((EXEC)pgm_read_word(&MAP_ARR[I8080::_IR])) ();				//decode

	} while (true);

	if (MEM::MEM_ERR) {
		MEM::MEM_ERR = false;
		CONSOLE::clrscr();
		CONSOLE::error("MEMORY ERROR!");
	}
}
