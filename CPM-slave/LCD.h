/**
 * Copyright (c) 2018 Roland K�rtvely <roland.kortvely@gmail.com>
 */


#ifndef _LCD_h
#define _LCD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

constexpr auto LCD_BLACK = 0x0000;
constexpr auto LCD_RED = 0xF800;
constexpr auto LCD_GREEN = 0x07E0;
constexpr auto LCD_WHITE = 0xFFFF;
constexpr auto LCD_GREY = 0x8410;
constexpr auto LCD_BLUE = 0x001F;
constexpr auto LCD_CYAN = 0x07FF;
constexpr auto LCD_MAGENTA = 0xF81F;
constexpr auto LCD_YELLOW = 0xFFE0;

class LCD
{
private:
	static void scroll();

public:
	static void init();
	static void welcome();

	static void write(uint8_t n);
	static void print(String s);
	static void println(String s = "");
	static void block(String s);
	static void blockln(String s);
	static void lnblockln(String s);
	static void ok();
	static void skip();
	static void error(String s);
	static void warning(String s);

	static void clrscr();
	static void color(uint8_t clr);
};

#endif
