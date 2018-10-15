/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 *
 * 27 - ESC
 */


#include "CONFIG.h"
#include "CONSOLE.h"

volatile uint8_t CONSOLE::kbd_chars = 0;

char CONSOLE::mon_buffer[MON_BUFFER_SIZE + 1];

boolean CONSOLE::MON = true;

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
		//Serial.flush();
		break;
	case 1: //PS2
		CONSOLE::kbd_chars = 0;
		break;
	}
}

void CONSOLE::welcome() {

	CONSOLE::color(2);

	CONSOLE::println(F("***************************************"));
	CONSOLE::println(F("*      (C) 2018 Roland Kortvely       *"));
	CONSOLE::println(F("***************************************"));
}

void CONSOLE::write(uint8_t n) {
	Serial.write(n);
}

void CONSOLE::print(String s) {
	Serial.print(s);
}

void CONSOLE::println(String s) {
	Serial.println(s);
}

void CONSOLE::block(String s) {
	Serial.print(s);
}

void CONSOLE::blockln(String s) {
	Serial.println(s + "\n");
}

void CONSOLE::lnblockln(String s) {
	Serial.println("\n" + s + "\n");
}

void CONSOLE::ok() {
	Serial.print(F("\t"));
	Serial.println(F("OK"));
}

void CONSOLE::skip() {
	Serial.print(F("\t"));
	Serial.println(F("SKIP"));
}

void CONSOLE::error() {

	CONSOLE::color(1);

	Serial.print(F("\t"));
	Serial.println(F("ERROR!"));
	while(1) {}								//TODO: RESTART
}

boolean CONSOLE::con_ready() {
	boolean res;
	switch (CON_IN) {
	case 0: //terminal
		if (Serial.available() > 0) {
			res = true;
		}
		else {
			res = false;
		}
		break;
	case 1: //PS/2 keyboard
			//cli();
		if (kbd_chars > 0) {
			res = true;
		}
		else {
			res = false;
		}
		//sei();
		break;
	}
	return res;
}

//console input/output procedures
char CONSOLE::con_read() {
	char key;
	key = '\0';
	switch (CON_IN) {
	case 0: //terminal
		if (Serial.available() > 0) {
			key = Serial.read();
			if (!MON && ((uint8_t)key == CTRL_SLASH_KEY)) {
				CONFIG::exitFlag = true;
			}
		}
		break;
	case 1: //PS/2 keyboard
	  //
		break;
	}
	return key;
}

//conversion functions
int CONSOLE::str2hex(String s)
{
	int x = 0;
	for (int i = 0; i < s.length(); i++) {
		char c = s.charAt(i);
		if (c >= '0' && c <= '9') {
			x *= 16;
			x += c - '0';
		}
		else if (c >= 'A' && c <= 'F') {
			x *= 16;
			x += (c - 'A') + 10;
		}
		else break;
	}
	return x;
}

uint8_t CONSOLE::chr2hex(char c)
{
	uint8_t x = 0;
	if (c >= '0' && c <= '9') {
		x *= 16;
		x += c - '0';
	}
	else if (c >= 'A' && c <= 'F') {
		x *= 16;
		x += (c - 'A') + 10;
	}
	return x;
}

boolean CONSOLE::hexcheck(uint8_t start, uint8_t len) {

	boolean ok = true;
	for (uint8_t i = start; i < (start + len); i++) {
		if (!(((CONSOLE::mon_buffer[i] >= '0') && (CONSOLE::mon_buffer[i] <= '9')) || ((CONSOLE::mon_buffer[i] >= 'A') && (CONSOLE::mon_buffer[i] <= 'F')))) {
			ok = false;
		}
	}
	return ok;
}

//keyboard input to numbers
uint8_t CONSOLE::kbd2nibble(uint8_t start) {
	return CONSOLE::chr2hex(CONSOLE::mon_buffer[start]);
}

uint8_t CONSOLE::kbd2byte(uint8_t start) {
	return CONSOLE::chr2hex(CONSOLE::mon_buffer[start]) * 16 + CONSOLE::chr2hex(CONSOLE::mon_buffer[start + 1]);
}

uint16_t CONSOLE::kbd2word(uint8_t start) {
	return (uint16_t)(CONSOLE::chr2hex(CONSOLE::mon_buffer[start]) * 16 * 16 * 16) + (uint16_t)(CONSOLE::chr2hex(CONSOLE::mon_buffer[start + 1]) * 16 * 16) + (uint16_t)(CONSOLE::chr2hex(CONSOLE::mon_buffer[start + 2]) * 16) + (uint16_t)(CONSOLE::chr2hex(CONSOLE::mon_buffer[start + 3]));
}

char CONSOLE::upCase(char symbol) {
	if (symbol >= 97 && symbol <= 122) {
		symbol = char(uint8_t(symbol) - 32);
	}
	return symbol;
}
