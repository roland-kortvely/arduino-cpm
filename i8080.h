/**
 * Copyright (c) 2018 Roland K�rtvely <roland.kortvely@gmail.com>
 */


#ifndef _I8080_h
#define _I8080_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

constexpr auto SZP_RESET = B00111011;
constexpr auto ZP_RESET = B10111011;

//SZ000P00  SZP flags lookup table
const static uint8_t PROGMEM SZP_table[] = {
  B01000100, B00000000, B00000000, B00000100, B00000000, B00000100, B00000100, B00000000, B00000000, B00000100, B00000100, B00000000, B00000100, B00000000, B00000000, B00000100,
  B00000000, B00000100, B00000100, B00000000, B00000100, B00000000, B00000000, B00000100, B00000100, B00000000, B00000000, B00000100, B00000000, B00000100, B00000100, B00000000,
  B00000000, B00000100, B00000100, B00000000, B00000100, B00000000, B00000000, B00000100, B00000100, B00000000, B00000000, B00000100, B00000000, B00000100, B00000100, B00000000,
  B00000100, B00000000, B00000000, B00000100, B00000000, B00000100, B00000100, B00000000, B00000000, B00000100, B00000100, B00000000, B00000100, B00000000, B00000000, B00000100,
  B00000000, B00000100, B00000100, B00000000, B00000100, B00000000, B00000000, B00000100, B00000100, B00000000, B00000000, B00000100, B00000000, B00000100, B00000100, B00000000,
  B00000100, B00000000, B00000000, B00000100, B00000000, B00000100, B00000100, B00000000, B00000000, B00000100, B00000100, B00000000, B00000100, B00000000, B00000000, B00000100,
  B00000100, B00000000, B00000000, B00000100, B00000000, B00000100, B00000100, B00000000, B00000000, B00000100, B00000100, B00000000, B00000100, B00000000, B00000000, B00000100,
  B00000000, B00000100, B00000100, B00000000, B00000100, B00000000, B00000000, B00000100, B00000100, B00000000, B00000000, B00000100, B00000000, B00000100, B00000100, B00000000,
  B10000000, B10000100, B10000100, B10000000, B10000100, B10000000, B10000000, B10000100, B10000100, B10000000, B10000000, B10000100, B10000000, B10000100, B10000100, B10000000,
  B10000100, B10000000, B10000000, B10000100, B10000000, B10000100, B10000100, B10000000, B10000000, B10000100, B10000100, B10000000, B10000100, B10000000, B10000000, B10000100,
  B10000100, B10000000, B10000000, B10000100, B10000000, B10000100, B10000100, B10000000, B10000000, B10000100, B10000100, B10000000, B10000100, B10000000, B10000000, B10000100,
  B10000000, B10000100, B10000100, B10000000, B10000100, B10000000, B10000000, B10000100, B10000100, B10000000, B10000000, B10000100, B10000000, B10000100, B10000100, B10000000,
  B10000100, B10000000, B10000000, B10000100, B10000000, B10000100, B10000100, B10000000, B10000000, B10000100, B10000100, B10000000, B10000100, B10000000, B10000000, B10000100,
  B10000000, B10000100, B10000100, B10000000, B10000100, B10000000, B10000000, B10000100, B10000100, B10000000, B10000000, B10000100, B10000000, B10000100, B10000100, B10000000,
  B10000000, B10000100, B10000100, B10000000, B10000100, B10000000, B10000000, B10000100, B10000100, B10000000, B10000000, B10000100, B10000000, B10000100, B10000100, B10000000,
  B10000100, B10000000, B10000000, B10000100, B10000000, B10000100, B10000100, B10000000, B10000000, B10000100, B10000100, B10000000, B10000100, B10000000, B10000000, B10000100,
};

class I8080
{
public:

	static uint8_t _Regs[8];
	/*
	  0 - B
	  1 - C
	  2 - D
	  3 - E
	  4 - H
	  5 - L
	  6 - M *Flags
	  7 - A
	*/

	static uint16_t breakpoint;

	static bool breakpointFlag;
	static bool INTE;

	//registers
	static volatile uint8_t _W;// W register
	static volatile uint8_t _Z;// Z register
	static volatile uint8_t _ACT;// ACT register
	static volatile uint8_t _TMP;// TMP register
	static volatile uint8_t _ALU;// ALU
	static volatile uint16_t _PC; //program counter
	static volatile uint16_t _SP; //stack pointer
	static volatile uint8_t _IR; //instruction register

	static void init();
	static void boot();
};

#endif