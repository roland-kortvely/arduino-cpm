/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "IPL.h"
#include "CONSOLE.h"
#include "IO.h"
#include "MEM.h"
#include "SD.h";
#include "FDD.h"

void IPL::init()
{
	CONFIG::init();

	CONSOLE::lnblockln(F("INITIAL PROGRAM LOAD (IPL)"));

	IO::init();

	MEM::init();
	SD::init();
	FDD::init();

	SD::MEM_TEST();

	CONSOLE::lnblockln("IPL DONE");
}
