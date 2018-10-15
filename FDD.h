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
	static uint32_t FDD::SD_FDD_OFFSET[FDD_NUM];

	static uint8_t  FDD_REG_SEC;					//sector register
	static uint8_t  FDD_REG_TRK;					//track register
	static uint8_t  FDD_REG_DRV;					//drive register
	static boolean  FDD_REG_STATUS;					//true - O.K., false - ERROR
	static uint16_t FDD_REG_DMA;					//DMA address register

	static void init();
};

#endif

