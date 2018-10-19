/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */

#include "GPU.h"
#include "CONSOLE.h"
#include "LCD.h"


void GPU::init()
{
	CONSOLE::init();
	LCD::init();
}

void GPU::write(uint8_t n) {
	CONSOLE::write(n);
	LCD::write(n);
}

void GPU::print(String s = "") {
	CONSOLE::print(s);
	LCD::print(s);
}

void GPU::println(String s = "") {
	CONSOLE::println(s);
	LCD::println(s);
}

void GPU::block(String s = "") {
	CONSOLE::print(s);
	LCD::print(s);
}

void GPU::blockln(String s = "") {
	CONSOLE::println(s + "\n");
	LCD::println(s + "\n");
}

void GPU::lnblockln(String s = "") {
	CONSOLE::println("\n" + s + "\n");
	LCD::println("\n" + s + "\n");
}

void GPU::ok() {
	CONSOLE::ok();
	LCD::ok();
}

void GPU::skip() {
	CONSOLE::skip();
	LCD::skip();
}

void GPU::warning(String s = "WARNING!") {
	CONSOLE::warning(s);
	LCD::warning(s);
}

void GPU::error(String s = "ERROR!") {
	CONSOLE::warning(s);
	LCD::warning(s);
}

void GPU::clrscr()
{
	CONSOLE::clrscr();
	LCD::clrscr();
}

void GPU::color(uint8_t c)
{
	CONSOLE::color(c);
	LCD::color(c);
}
