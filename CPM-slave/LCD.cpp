/**
 * Copyright (c) 2018 Roland K�rtvely <roland.kortvely@gmail.com>
 */


#include <MCUFRIEND_kbv.h>							// Hardware-specific library


#include "LCD.h"

MCUFRIEND_kbv tft;

int height;

void LCD::scroll()
{
	if (tft.getCursorY() > 236) {
		LCD::clrscr();
		tft.setCursor(0, 0);
	}
}

void LCD::init()
{
	tft.begin(tft.readID());
	tft.setRotation(3);						//LANDSCAPE

	height = tft.height();

	tft.setTextSize(1);

	LCD::clrscr();
	LCD::welcome();
}

void LCD::welcome()
{
	LCD::color(2);

	LCD::println(F("***************************************"));
	LCD::println(F("*      (C) 2018 Roland Kortvely       *"));
	LCD::println(F("***************************************"));
}

void LCD::write(uint8_t n) {
	tft.write(n);
	LCD::scroll();
}

void LCD::print(String s = "") {
	tft.print(s);
	LCD::scroll();
}

void LCD::println(String s = "")
{
	tft.println(s);
	LCD::scroll();
}

void LCD::block(String s = "") {
	tft.print(s);
	LCD::scroll();
}

void LCD::blockln(String s = "") {
	tft.println(s + "\n");
	LCD::scroll();
}

void LCD::lnblockln(String s = "") {
	tft.println("\n" + s + "\n");
	LCD::scroll();
}

void LCD::ok() {
	tft.print(F("    "));
	tft.println(F("OK"));
	LCD::scroll();
}

void LCD::skip() {
	tft.print(F("    "));
	tft.println(F("SKIP"));
	LCD::scroll();
}

void LCD::error(String s = "ERROR!") {

	LCD::color(1);

	tft.print(F("    "));
	tft.println(s);
	LCD::scroll();
}

void LCD::warning(String s = "WARNING!") {
	LCD::color(5);
	tft.println(s);
	LCD::color(9);
	LCD::scroll();
}

void LCD::clrscr()
{
	tft.setCursor(0, 0);
	tft.fillScreen(LCD_BLACK);
}

void LCD::color(uint8_t clr)
{
	switch (clr) {
	case 1:
		tft.setTextColor(LCD_RED);
		break;
	case 2:
		tft.setTextColor(LCD_GREEN);
		break;
	case 5:
		tft.setTextColor(LCD_CYAN);
		break;
	default:
		tft.setTextColor(LCD_WHITE);
	}
}
