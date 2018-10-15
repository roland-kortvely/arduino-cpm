/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "CONFIG.h"

#ifndef _CONSOLE_h
#define _CONSOLE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class CONSOLE
{
public:

	static volatile char kbd_buffer[KBD_BUFFER_SIZE];	//console input buffer
	static volatile uint8_t kbd_chars;				//received chars number

	static char mon_buffer[MON_BUFFER_SIZE + 1];
	static char inChar;

	static int mon_ptr;

	static boolean MON;

	static void clrscr();
	static void clrlin();
	static void clrend();
	static void clrarea();
	static void savecur();
	static void loadcur();
	static void color(uint8_t clr);
	static void xy(uint8_t row, uint8_t col);

	static void con_flush();
	static boolean con_ready();
	static char con_read();
	static int str2hex(String s);
	static uint8_t chr2hex(char c);
	static boolean hexcheck(uint8_t start, uint8_t len);
	static uint8_t kbd2nibble(uint8_t start);
	static uint8_t kbd2byte(uint8_t start);
	static uint16_t kbd2word(uint8_t start);
	static char upCase(char symbol);

	static void welcome();

	static void write(uint8_t n);
	static void print(String s = "");
	static void println(String s = "");
	static void block(String s = "");
	static void blockln(String s = "");
	static void lnblockln(String s = "");
	static void ok();
	static void skip();
	static void error();
};

#endif
