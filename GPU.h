/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#ifndef _GPU_h
#define _GPU_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class GPU
{
 public:
	static void init();

	static void write(uint8_t n = 0);
	static void print(String s = "");
	static void println(String s = "");
	static void block(String s = "");
	static void blockln(String s = "");
	static void lnblockln(String s = "");
	static void ok();
	static void skip();
	static void error(String s = "ERROR!");
	static void warning(String s = "WARNING!");

	static void clrscr();
	static void color(uint8_t c);
};

#endif
