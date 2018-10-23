/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#ifndef _h
#define _h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

constexpr auto SZP_RESET = B00111011;
constexpr auto ZP_RESET = B10111011;

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

extern uint8_t _Regs[8];
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


//SZ000P00  SZP flags lookup table
const static uint8_t PROGMEM SZP_table[] = {
  B01000100, B00000000, B00000000, B00000100, B00000000, B00000100, B00000100, B00000000, B00000000, B00000100, B00000100, B00000000, B00000100, B00000000, B00000000, B00000100,
  B00000000, B00000100, B00000100, B00000000, B00000100, B00000000, B00000000, B00000100, B00000100, B00000000, B00000000, B00000100, B00000000, B00000100, B00000100, B00000000,
  B00000000, B00000100, B00000100, B00000000, B00000100, B00000000, B00000000, B00000100, B00000100, B00000000, B00000000, B00000100, B00000000, B00000100, B00000100, B00000000,
  B00000100, B00000000, B00000000, B00000100, B00000000, B00000100, B00000100, B00000000, B00000000, B00000100, B00000100, B00000000, B00000100, B00000000, B00000000, B00000100,
  B00000000, B00000100, B00000100, B00000000, B00000100, B00000000, B00000000, B00000100, B00000100, B00000000, B00000000, B00000100, B00000000, B00000100, B00000100, B00000000,
  B00000100, B00000000, B00000000, B00000100, B00000000, B00000100, B00000100, B00000000, B00000000, B00000100, B00000100, B00000000, B00000100, B00000000, B00000000, B00000100,
  B00000100, B00000000, B00000000, B00000100, B00000000, B00000100, B00000100, B00000000, B00000000, B00000100, B00000100, B00000000, B00000100, B00000000, B00000000, B00000100,
  B00000000, B00000100, B00000100, B00000000, B00000100, B00000000, B00000000, B00000100, B00000100, B00000000, B00000000, B00000100, B00000000, B00000100, B00000100, B00000000,
  B10000000, B10000100, B10000100, B10000000, B10000100, B10000000, B10000000, B10000100, B10000100, B10000000, B10000000, B10000100, B10000000, B10000100, B10000100, B10000000,
  B10000100, B10000000, B10000000, B10000100, B10000000, B10000100, B10000100, B10000000, B10000000, B10000100, B10000100, B10000000, B10000100, B10000000, B10000000, B10000100,
  B10000100, B10000000, B10000000, B10000100, B10000000, B10000100, B10000100, B10000000, B10000000, B10000100, B10000100, B10000000, B10000100, B10000000, B10000000, B10000100,
  B10000000, B10000100, B10000100, B10000000, B10000100, B10000000, B10000000, B10000100, B10000100, B10000000, B10000000, B10000100, B10000000, B10000100, B10000100, B10000000,
  B10000100, B10000000, B10000000, B10000100, B10000000, B10000100, B10000100, B10000000, B10000000, B10000100, B10000100, B10000000, B10000100, B10000000, B10000000, B10000100,
  B10000000, B10000100, B10000100, B10000000, B10000100, B10000000, B10000000, B10000100, B10000100, B10000000, B10000000, B10000100, B10000000, B10000100, B10000100, B10000000,
  B10000000, B10000100, B10000100, B10000000, B10000100, B10000000, B10000000, B10000100, B10000100, B10000000, B10000000, B10000100, B10000000, B10000100, B10000100, B10000000,
  B10000100, B10000000, B10000000, B10000100, B10000000, B10000100, B10000100, B10000000, B10000000, B10000100, B10000100, B10000000, B10000100, B10000000, B10000000, B10000100,
};

class I8080
{
public:

	static uint16_t breakpoint;

	static bool breakpointFlag;
	static bool INTE;

	//registers
	static volatile uint8_t _W;					// W register
	static volatile uint8_t _Z;					// Z register
	static volatile uint8_t _ACT;				// ACT register
	static volatile uint8_t _TMP;				// TMP register
	static volatile uint8_t _ALU;				// ALU
	static volatile uint16_t _PC;				//program counter
	static volatile uint16_t _SP;				//stack pointer
	static volatile uint8_t _IR;				//instruction register

	static void init();
	static void BOOT();

	static void state();
	static uint16_t pc2a16();
	static void pc2sp();

	static void _();							//NOP
	static void _HLT();
	static void _MOV(uint8_t DDD, uint8_t SSS);
	static void _MVI(uint8_t DDD);
	static void _LXI(uint8_t rp);
	static void _LDA();
	static void _STA();
	static void _LHLD();
	static void _SHLD();
	static void _LDAX(uint8_t rp);
	static void _STAX(uint8_t rp);
	static void _XCHG();
	static void _ADD(uint8_t SSS);
	static void _ADI();
	static void _ADC(uint8_t SSS);
	static void _ACI();
	static void _SUB(uint8_t SSS);
	static void _SUI();
	static void _SBB(uint8_t SSS);
	static void _SBI();
	static void _INR(uint8_t DDD);
	static void _DCR(uint8_t DDD);
	static void _INX(uint8_t rp);
	static void _DCX(uint8_t rp);
	static void _DAD(uint8_t rp);
	static void _DAA();
	static void _ANA(uint8_t SSS);
	static void _ANI();
	static void _ORA(uint8_t SSS);
	static void _ORI();
	static void _XRA(uint8_t SSS);
	static void _XRI();
	static void _CMP(uint8_t SSS);
	static void _CPI();
	static void _RRC();
	static void _RLC();
	static void _RAL();
	static void _RAR();
	static void _CMA();
	static void _CMC();
	static void _STC();
	static void _JMP();
	static void _JCCC(uint8_t CCC);
	static void _CALL();
	static void _CCCC(uint8_t CCC);
	static void _RET();
	static void _RCCC(uint8_t CCC);
	static void _RST(uint8_t n);
	static void _PCHL();
	static void _XTHL();
	static void _SPHL();
	static void _PUSH(uint8_t rp);
	static void _POP(uint8_t rp);
	static void _IN();
	static void _OUT();
	static void _EI();
	static void _DI();
};

#endif
