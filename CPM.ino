/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "CONFIG.h"
#include "CONSOLE.h"
#include "IO.h"
#include "IPL.h"

void setup()
{
	CONFIG::init();
	IO::init();
	IPL::init();
}

void loop()
{

}
