/**
 * Copyright (c) 2018 Roland K?rtvely <roland.kortvely@gmail.com>
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
	static void MEM_TEST();
	static void transfer();
	static void BIOS();

 public:
	static void exec();
};

#endif

