/**
 * Copyright (c) 2018 Roland K�rtvely <roland.kortvely@gmail.com>
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
