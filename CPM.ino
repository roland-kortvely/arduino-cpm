/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */

#include "CONFIG.h"
#include "IPL.h"
#include "i8080.h"

void setup()
{
	IPL::init();
	I8080::init();
}

void loop()
{
	I8080::BOOT();
	HALT
}
