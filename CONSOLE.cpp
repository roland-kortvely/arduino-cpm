/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 *
 * 27 - ESC
 */


#include "CONFIG.h"
#include "CONSOLE.h"

volatile uint8_t CONSOLE::kbd_chars = 0;

void CONSOLE::clrscr()
{
	Serial.write(27);
	Serial.print("[2J");
	Serial.write(27);
	Serial.print("[H");
}

void CONSOLE::clrlin()
{
	Serial.write(27);
	Serial.print("[2K");
}

void CONSOLE::clrend()
{
	Serial.write(27);
	Serial.print("[K");
}


void CONSOLE::clrarea()
{
	Serial.write(27);
	Serial.print("[J");
}

void CONSOLE::savecur()
{
	Serial.write(27);
	Serial.print("[s");
}

void CONSOLE::loadcur()
{
	Serial.write(27);
	Serial.print("[u");
}

void CONSOLE::color(uint8_t clr)
{
	Serial.write(27);
	Serial.print("[");
	switch (clr) {
	default:
	case 0: Serial.print("30");
		break;
	case 1: Serial.print("31");
		break;
	case 2: Serial.print("32");
		break;
	case 3: Serial.print("33");
		break;
	case 4: Serial.print("34");
		break;
	case 5: Serial.print("35");
		break;
	case 6: Serial.print("36");
		break;
	case 7: Serial.print("37");
		break;
	case 8: Serial.print("38");
		break;
	case 9: Serial.print("39");
		break;
	}
	Serial.print("m");
}

void CONSOLE::xy(uint8_t row, uint8_t col) {
	Serial.write(27);
	Serial.print("[");
	Serial.print(row, DEC);
	Serial.print(";");
	Serial.print(col, DEC);
	Serial.print("H");
}

void CONSOLE::con_flush() {
	switch (CON_IN) {
	case 0: //terminal
		Serial.flush();
		break;
	case 1: //PS2
		CONSOLE::kbd_chars = 0;
		break;
	}
}

void CONSOLE::welcome() {
	CONSOLE::color(2);

	Serial.println(F("***************************************"));
	Serial.println(F("*      (C) 2018 Roland Kortvely       *"));
	Serial.println(F("***************************************"));
	Serial.println("");
}