/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "CONFIG.h"

#ifndef _FDD_h
#define _FDD_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class FDD
{
public:
	static  uint32_t FDD::SD_FDD_OFFSET[FDD_NUM];
	static void init();
};

#endif

