/**
 * Copyright (c) 2018 Roland K�rtvely <roland.kortvely@gmail.com>
 */


#include "CONFIG.h"
#include "CONSOLE.h"
#include "i8080.h"

uint8_t I8080::_Regs[8];
/*
  0 - B
  1 - C
  2 - D
  3 - E
  4 - H
  5 - L
  6 - M *Flags
  7 - A
*/

#define _Reg_B B000
#define _Reg_C B001
#define _Reg_D B010
#define _Reg_E B011
#define _Reg_H B100
#define _Reg_L B101
#define _Reg_M B110
#define _Reg_A B111

#define _RP_BC B00
#define _RP_DE B01
#define _RP_HL B10
#define _RP_SP B11
#define _RP_AF B11

#define _rA _Regs[_Reg_A]
#define _rB _Regs[_Reg_B]
#define _rC _Regs[_Reg_C]
#define _rD _Regs[_Reg_D]
#define _rE _Regs[_Reg_E]
#define _rH _Regs[_Reg_H]
#define _rL _Regs[_Reg_L]
#define _rF _Regs[_Reg_M]
#define _rW _W
#define _rZ _Z
#define _rpAF word(_rA, _rF);
#define _rpBC word(_rB, _rC);
#define _rpDE word(_rD, _rE);
#define _rpHL word(_rH, _rL);
#define _rpWZ word(_rW, _rZ);

#define _setFlags_S(b) _Regs[_Reg_M] = (b==1) ? (_Regs[_Reg_M] | B10000000) : (_Regs[_Reg_M] & B01111111)
#define _setFlags_Z(b) _Regs[_Reg_M] = (b==1) ? (_Regs[_Reg_M] | B01000000) : (_Regs[_Reg_M] & B10111111)
#define _setFlags_A(b) _Regs[_Reg_M] = (b==1) ? (_Regs[_Reg_M] | B00010000) : (_Regs[_Reg_M] & B11101111)
#define _setFlags_P(b) _Regs[_Reg_M] = (b==1) ? (_Regs[_Reg_M] | B00000100) : (_Regs[_Reg_M] & B11111011)
#define _setFlags_C(b) _Regs[_Reg_M] = (b==1) ? (_Regs[_Reg_M] | B00000001) : (_Regs[_Reg_M] & B11111110)

#define _getFlags_S()  (((_Regs[_Reg_M] & B10000000) != 0) ? (uint8_t(1)) : (uint8_t(0)))
#define _getFlags_Z()  (((_Regs[_Reg_M] & B01000000) != 0) ? (uint8_t(1)) : (uint8_t(0)))
#define _getFlags_A()  (((_Regs[_Reg_M] & B00010000) != 0) ? (uint8_t(1)) : (uint8_t(0)))
#define _getFlags_P()  (((_Regs[_Reg_M] & B00000100) != 0) ? (uint8_t(1)) : (uint8_t(0)))
#define _getFlags_C()  (((_Regs[_Reg_M] & B00000001) != 0) ? (uint8_t(1)) : (uint8_t(0)))

#define _FLAGS(b) _rF = (_rF & SZP_RESET) | pgm_read_byte_near(SZP_table + b);

uint16_t I8080::breakpoint = 0xFFFF;

bool I8080::breakpointFlag = false;
bool I8080::INTE;

//registers
volatile uint8_t  I8080::_W;		// W register
volatile uint8_t  I8080::_Z;		// Z register
volatile uint8_t  I8080::_ACT;		// ACT register
volatile uint8_t  I8080::_TMP;		// TMP register
volatile uint8_t  I8080::_ALU;		// ALU
volatile uint16_t I8080::_PC;		//program counter
volatile uint16_t I8080::_SP;		//stack pointer
volatile uint8_t  I8080::_IR;		//instruction register

void I8080::init()
{
	CONSOLE::color(9);
	
	I8080::_SP = SP_INIT;			//stack init

	delay(1000);					//just for UX		TODO: ASYNC delay

	CONSOLE::clrscr();				//clear screen
	CONSOLE::xy(MON_Y, 0);			//cursor positioning

	CONSOLE::print(">");
}

void I8080::boot() {

}
