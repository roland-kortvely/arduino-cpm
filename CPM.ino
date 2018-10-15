/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "IPL.h"
#include "i8080.h"
#include "CONSOLE.h"

void setup()
{
	IPL::init();
	I8080::init();
}

void loop()
{
	I8080::boot();

	CONSOLE::clrscr();
	CONSOLE::color(1);
	CONSOLE::lnblockln("HALT!");
	
	while (1) {}						//TODO: Reset
}
