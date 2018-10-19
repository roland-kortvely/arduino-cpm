/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include <Wire.h>

#include "CONFIG.h"
#include "CONSOLE.h"
#include "LCD.h"


void LCD::init()
{
	Wire.begin();

	delay(2000);			//Wait for Slave arduino init		//TODO:: Handshake

	LCD::clrscr();
}

void LCD::write(uint8_t n) {
	Wire.beginTransmission(8);
	Wire.write(n);      
	Wire.endTransmission();
}

void LCD::print(String s = "") {
	Wire.beginTransmission(8);
	for (size_t i = 0; i < s.length(); i++)
	{
		Wire.write(s.charAt(i));
	}
	Wire.endTransmission();
}

void LCD::println(String s = "")
{
	Wire.beginTransmission(8);
	for (size_t i = 0; i < s.length(); i++)
	{
		Wire.write(s.charAt(i));
	}
	Wire.write('\n');
	Wire.endTransmission();
}

void LCD::block(String s = "") {
	LCD::print(s);
	/*
	tft.print(s);
	LCD::scroll();
	*/
}

void LCD::blockln(String s = "") {
	LCD::print(s + "\n");
	/*
	tft.println(s + "\n");
	LCD::scroll();
	*/
}

void LCD::lnblockln(String s = "") {
	LCD::println("\n" + s + "\n");
	/*
	tft.println("\n" + s + "\n");
	LCD::scroll();
	*/
}

void LCD::ok() {
	LCD::println("    OK");
	/*
	tft.print(F("    "));
	tft.println(F("OK"));
	LCD::scroll();
	*/
}

void LCD::skip() {
	LCD::println("    SKIP");
	/*
	tft.print(F("    "));
	tft.println(F("SKIP"));
	LCD::scroll();
	*/
}

void LCD::error(String s = "ERROR!") {
	LCD::println(s);
	/*
	LCD::color(1);

	tft.print(F("    "));
	tft.println(s);
	LCD::scroll();
	*/
}

void LCD::warning(String s = "WARNING!") {
	LCD::println(s);
	/*
	LCD::color(5);
	tft.println(s);
	LCD::color(9);
	LCD::scroll();
	*/
}

void LCD::clrscr()
{
	/*
	tft.setCursor(0, 0);
	tft.fillScreen(LCD_BLACK);
	*/
}

void LCD::color(uint8_t clr)
{
	/*
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
	*/
}
