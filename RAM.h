/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#ifndef _RAM_h
#define _RAM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class RAM
{
private:
	static void ERASE();
 public:
	static void init();
	static void MEM_TEST();
};

#endif

