/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#ifndef _CMD_h
#define _CMD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class CMD
{
private:
	static void format();
	static void status();
	static void BIOS();
	static void MEM_TEST();
 public:
	static void exec();
};

#endif

