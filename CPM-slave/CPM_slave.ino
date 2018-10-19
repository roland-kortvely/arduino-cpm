/**
 * Copyright (c) 2018 Roland K�rtvely <roland.kortvely@gmail.com>
 */


#include <Wire.h>

#include "LCD.h"


void setup()
{
	LCD::init();

	Wire.begin(8);             
	Wire.onReceive(receiveEvent);

	Serial.begin(9600);
}

void receiveEvent(int count) {
	while (Wire.available()) {
		char c = Wire.read();
		LCD::write(c);
		Serial.print(c);
	}
}

void loop()
{
	
}
