/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "CONFIG.h"
#include "IPL.h"
#include "GPU.h"
#include "IO.h"
#include "MEM.h"
#include "SD.h";
#include "FDD.h"
#include "RAM.h"

void IPL::init()
{
	GPU::init();

	//GPU::lnblockln(F("INITIAL PROGRAM LOAD (IPL)"));

	IO::init();

	MEM::init();
	SD::init();
	FDD::init();

	RAM::init();
}
