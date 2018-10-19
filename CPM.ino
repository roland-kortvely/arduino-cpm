/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "IPL.h"
#include "BIOS.h"

void setup()
{
	IPL::init();
}

void loop()
{
	BIOS::BOOT();
}
