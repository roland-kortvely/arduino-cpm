/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */

#include "CONFIG.h"
#include "BIOS.h"
#include "i8080.h"

#ifndef _HELPER_h
#define _HELPER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void do0x00() {
	I8080::_();				//*NOP
}
void do0x01() {
	I8080::_LXI(_RP_BC);
}
void do0x02() {
	I8080::_STAX(_RP_BC);
}
void do0x03() {
	I8080::_INX(_Reg_B);
}
void do0x04() {
	I8080::_INR(_Reg_B);
}
void do0x05() {
	I8080::_DCR(_Reg_B);
}
void do0x06() {
	I8080::_MVI(_Reg_B);
}
void do0x07() {
	I8080::_RLC();
}
void do0x08() {
	I8080::_();				//*NOP
}
void do0x09() {
	I8080::_DAD(_RP_BC);
}
void do0x0A() {
	I8080::_LDAX(_RP_BC);
}
void do0x0B() {
	I8080::_DCX(_Reg_B);
}
void do0x0C() {
	I8080::_INR(_Reg_C);
}
void do0x0D() {
	I8080::_DCR(_Reg_C);
}
void do0x0E() {
	I8080::_MVI(_Reg_C);
}
void do0x0F() {
	I8080::_RRC();
}

//10
void do0x10() {
	I8080::_();				//*NOP
}
void do0x11() {
	I8080::_LXI(_RP_DE);
}
void do0x12() {
	I8080::_STAX(_RP_DE);
}
void do0x13() {
	I8080::_INX(_RP_DE);
}
void do0x14() {
	I8080::_INR(_Reg_D);
}
void do0x15() {
	I8080::_DCR(_Reg_D);
}
void do0x16() {
	I8080::_MVI(_Reg_D);
}
void do0x17() {
	I8080::_RAL();
}
void do0x18() {
	I8080::_();				//*NOP
}
void do0x19() {
	I8080::_DAD(_RP_DE);
}
void do0x1A() {
	I8080::_LDAX(_RP_DE);
}
void do0x1B() {
	I8080::_DCX(_RP_DE);
}
void do0x1C() {
	I8080::_INR(_Reg_E);
}
void do0x1D() {
	I8080::_DCR(_Reg_E);
}
void do0x1E() {
	I8080::_MVI(_Reg_E);
}
void do0x1F() {
	I8080::_RAR();
}

//20
void do0x20() {
	I8080::_();				//*NOP
}
void do0x21() {
	I8080::_LXI(_RP_HL);
}
void do0x22() {
	I8080::_SHLD();
}
void do0x23() {
	I8080::_INX(_RP_HL);
}
void do0x24() {
	I8080::_INR(_Reg_H);
}
void do0x25() {
	I8080::_DCR(_Reg_H);
}
void do0x26() {
	I8080::_MVI(_Reg_H);
}
void do0x27() {
	I8080::_DAA();
}
void do0x28() {
	I8080::_();//*NOP
}
void do0x29() {
	I8080::_DAD(_RP_HL);
}
void do0x2A() {
	I8080::_LHLD();
}
void do0x2B() {
	I8080::_DCX(_RP_HL);
}
void do0x2C() {
	I8080::_INR(_Reg_L);
}
void do0x2D() {
	I8080::_DCR(_Reg_L);
}
void do0x2E() {
	I8080::_MVI(_Reg_L);
}
void do0x2F() {
	I8080::_CMA();
}


//30
void do0x30() {
	I8080::_();//*NOP
}
void do0x31() {
	I8080::_LXI(_RP_SP);
}
void do0x32() {
	I8080::_STA();
}
void do0x33() {
	I8080::_INX(_RP_SP);
}
void do0x34() {
	I8080::_INR(_Reg_M);
}
void do0x35() {
	I8080::_DCR(_Reg_M);
}
void do0x36() {
	I8080::_MVI(_Reg_M);
}
void do0x37() {
	I8080::_STC();
}
void do0x38() {
	I8080::_();//*NOP
}
void do0x39() {
	I8080::_DAD(_RP_SP);
}
void do0x3A() {
	I8080::_LDA();
}
void do0x3B() {
	I8080::_DCX(_RP_SP);
}
void do0x3C() {
	I8080::_INR(_Reg_A);
}
void do0x3D() {
	I8080::_DCR(_Reg_A);
}
void do0x3E() {
	I8080::_MVI(_Reg_A);
}
void do0x3F() {
	I8080::_CMC();
}


//40 MOV dest src
void do0x40() {
	I8080::_MOV(_Reg_B, _Reg_B);
}
void do0x41() {
	I8080::_MOV(_Reg_B, _Reg_C);
}
void do0x42() {
	I8080::_MOV(_Reg_B, _Reg_D);
}
void do0x43() {
	I8080::_MOV(_Reg_B, _Reg_E);
}
void do0x44() {
	I8080::_MOV(_Reg_B, _Reg_H);
}
void do0x45() {
	I8080::_MOV(_Reg_B, _Reg_L);
}
void do0x46() {
	I8080::_MOV(_Reg_B, _Reg_M);
}
void do0x47() {
	I8080::_MOV(_Reg_B, _Reg_A);
}
void do0x48() {
	I8080::_MOV(_Reg_C, _Reg_B);
}
void do0x49() {
	I8080::_MOV(_Reg_C, _Reg_C);
}
void do0x4A() {
	I8080::_MOV(_Reg_C, _Reg_D);
}
void do0x4B() {
	I8080::_MOV(_Reg_C, _Reg_E);
}
void do0x4C() {
	I8080::_MOV(_Reg_C, _Reg_H);
}
void do0x4D() {
	I8080::_MOV(_Reg_C, _Reg_L);
}
void do0x4E() {
	I8080::_MOV(_Reg_C, _Reg_M);
}
void do0x4F() {
	I8080::_MOV(_Reg_C, _Reg_A);
}

//50
void do0x50() {
	I8080::_MOV(_Reg_D, _Reg_B);
}
void do0x51() {
	I8080::_MOV(_Reg_D, _Reg_C);
}
void do0x52() {
	I8080::_MOV(_Reg_D, _Reg_D);
}
void do0x53() {
	I8080::_MOV(_Reg_D, _Reg_E);
}
void do0x54() {
	I8080::_MOV(_Reg_D, _Reg_H);
}
void do0x55() {
	I8080::_MOV(_Reg_D, _Reg_L);
}
void do0x56() {
	I8080::_MOV(_Reg_D, _Reg_M);
}
void do0x57() {
	I8080::_MOV(_Reg_D, _Reg_A);
}
void do0x58() {
	I8080::_MOV(_Reg_E, _Reg_B);
}
void do0x59() {
	I8080::_MOV(_Reg_E, _Reg_C);
}
void do0x5A() {
	I8080::_MOV(_Reg_E, _Reg_D);
}
void do0x5B() {
	I8080::_MOV(_Reg_E, _Reg_E);
}
void do0x5C() {
	I8080::_MOV(_Reg_E, _Reg_H);
}
void do0x5D() {
	I8080::_MOV(_Reg_E, _Reg_L);
}
void do0x5E() {
	I8080::_MOV(_Reg_E, _Reg_M);
}
void do0x5F() {
	I8080::_MOV(_Reg_E, _Reg_A);
}

//60
void do0x60() {
	I8080::_MOV(_Reg_H, _Reg_B);
}
void do0x61() {
	I8080::_MOV(_Reg_H, _Reg_C);
}
void do0x62() {
	I8080::_MOV(_Reg_H, _Reg_D);
}
void do0x63() {
	I8080::_MOV(_Reg_H, _Reg_E);
}
void do0x64() {
	I8080::_MOV(_Reg_H, _Reg_H);
}
void do0x65() {
	I8080::_MOV(_Reg_H, _Reg_L);
}
void do0x66() {
	I8080::_MOV(_Reg_H, _Reg_M);
}
void do0x67() {
	I8080::_MOV(_Reg_H, _Reg_A);
}
void do0x68() {
	I8080::_MOV(_Reg_L, _Reg_B);
}
void do0x69() {
	I8080::_MOV(_Reg_L, _Reg_C);
}
void do0x6A() {
	I8080::_MOV(_Reg_L, _Reg_D);
}
void do0x6B() {
	I8080::_MOV(_Reg_L, _Reg_E);
}
void do0x6C() {
	I8080::_MOV(_Reg_L, _Reg_H);
}
void do0x6D() {
	I8080::_MOV(_Reg_L, _Reg_L);
}
void do0x6E() {
	I8080::_MOV(_Reg_L, _Reg_M);
}
void do0x6F() {
	I8080::_MOV(_Reg_L, _Reg_A);
}

//70
void do0x70() {
	I8080::_MOV(_Reg_M, _Reg_B);
}
void do0x71() {
	I8080::_MOV(_Reg_M, _Reg_C);
}
void do0x72() {
	I8080::_MOV(_Reg_M, _Reg_D);
}
void do0x73() {
	I8080::_MOV(_Reg_M, _Reg_E);
}
void do0x74() {
	I8080::_MOV(_Reg_M, _Reg_H);
}
void do0x75() {
	I8080::_MOV(_Reg_M, _Reg_L);
}
void do0x76() {
	I8080::_HLT();
}
void do0x77() {
	I8080::_MOV(_Reg_M, _Reg_A);
}
void do0x78() {
	I8080::_MOV(_Reg_A, _Reg_B);
}
void do0x79() {
	I8080::_MOV(_Reg_A, _Reg_C);
}
void do0x7A() {
	I8080::_MOV(_Reg_A, _Reg_D);
}
void do0x7B() {
	I8080::_MOV(_Reg_A, _Reg_E);
}
void do0x7C() {
	I8080::_MOV(_Reg_A, _Reg_H);
}
void do0x7D() {
	I8080::_MOV(_Reg_A, _Reg_L);
}
void do0x7E() {
	I8080::_MOV(_Reg_A, _Reg_M);
}
void do0x7F() {
	I8080::_MOV(_Reg_A, _Reg_A);
}


//80
void do0x80() {
	I8080::_ADD(_Reg_B);
}
void do0x81() {
	I8080::_ADD(_Reg_C);
}
void do0x82() {
	I8080::_ADD(_Reg_D);
}
void do0x83() {
	I8080::_ADD(_Reg_E);
}
void do0x84() {
	I8080::_ADD(_Reg_H);
}
void do0x85() {
	I8080::_ADD(_Reg_L);
}
void do0x86() {
	I8080::_ADD(_Reg_M);
}
void do0x87() {
	I8080::_ADD(_Reg_A);
}
void do0x88() {
	I8080::_ADC(_Reg_B);
}
void do0x89() {
	I8080::_ADC(_Reg_C);
}
void do0x8A() {
	I8080::_ADC(_Reg_D);
}
void do0x8B() {
	I8080::_ADC(_Reg_E);
}
void do0x8C() {
	I8080::_ADC(_Reg_H);
}
void do0x8D() {
	I8080::_ADC(_Reg_L);
}
void do0x8E() {
	I8080::_ADC(_Reg_M);
}
void do0x8F() {
	I8080::_ADC(_Reg_A);
}

//90
void do0x90() {
	I8080::_SUB(_Reg_B);
}
void do0x91() {
	I8080::_SUB(_Reg_C);
}
void do0x92() {
	I8080::_SUB(_Reg_D);
}
void do0x93() {
	I8080::_SUB(_Reg_E);
}
void do0x94() {
	I8080::_SUB(_Reg_H);
}
void do0x95() {
	I8080::_SUB(_Reg_L);
}
void do0x96() {
	I8080::_SUB(_Reg_M);
}
void do0x97() {
	I8080::_SUB(_Reg_A);
}
void do0x98() {
	I8080::_SBB(_Reg_B);
}
void do0x99() {
	I8080::_SBB(_Reg_C);
}
void do0x9A() {
	I8080::_SBB(_Reg_D);
}
void do0x9B() {
	I8080::_SBB(_Reg_E);
}
void do0x9C() {
	I8080::_SBB(_Reg_H);
}
void do0x9D() {
	I8080::_SBB(_Reg_L);
}
void do0x9E() {
	I8080::_SBB(_Reg_M);
}
void do0x9F() {
	I8080::_SBB(_Reg_A);
}

//A0
void do0xA0() {
	I8080::_ANA(_Reg_B);
}
void do0xA1() {
	I8080::_ANA(_Reg_C);
}
void do0xA2() {
	I8080::_ANA(_Reg_D);
}
void do0xA3() {
	I8080::_ANA(_Reg_E);
}
void do0xA4() {
	I8080::_ANA(_Reg_H);
}
void do0xA5() {
	I8080::_ANA(_Reg_L);
}
void do0xA6() {
	I8080::_ANA(_Reg_M);
}
void do0xA7() {
	I8080::_ANA(_Reg_A);
}
void do0xA8() {
	I8080::_XRA(_Reg_B);
}
void do0xA9() {
	I8080::_XRA(_Reg_C);
}
void do0xAA() {
	I8080::_XRA(_Reg_D);
}
void do0xAB() {
	I8080::_XRA(_Reg_E);
}
void do0xAC() {
	I8080::_XRA(_Reg_H);
}
void do0xAD() {
	I8080::_XRA(_Reg_L);
}
void do0xAE() {
	I8080::_XRA(_Reg_M);
}
void do0xAF() {
	I8080::_XRA(_Reg_A);
}

//B0
void do0xB0() {
	I8080::_ORA(_Reg_B);
}
void do0xB1() {
	I8080::_ORA(_Reg_C);
}
void do0xB2() {
	I8080::_ORA(_Reg_D);
}
void do0xB3() {
	I8080::_ORA(_Reg_E);
}
void do0xB4() {
	I8080::_ORA(_Reg_H);
}
void do0xB5() {
	I8080::_ORA(_Reg_L);
}
void do0xB6() {
	I8080::_ORA(_Reg_M);
}
void do0xB7() {
	I8080::_ORA(_Reg_A);
}
void do0xB8() {
	I8080::_CMP(_Reg_B);
}
void do0xB9() {
	I8080::_CMP(_Reg_C);
}
void do0xBA() {
	I8080::_CMP(_Reg_D);
}
void do0xBB() {
	I8080::_CMP(_Reg_E);
}
void do0xBC() {
	I8080::_CMP(_Reg_H);
}
void do0xBD() {
	I8080::_CMP(_Reg_L);
}
void do0xBE() {
	I8080::_CMP(_Reg_M);
}
void do0xBF() {
	I8080::_CMP(_Reg_A);
}

//C0
void do0xC0() {
	I8080::_RCCC(B000);
}
void do0xC1() {
	I8080::_POP(_RP_BC);
}
void do0xC2() {
	I8080::_JCCC(B000);
}
void do0xC3() {
	I8080::_JMP();
}
void do0xC4() {
	I8080::_CCCC(B000);
}
void do0xC5() {
	I8080::_PUSH(_RP_BC);
}
void do0xC6() {
	I8080::_ADI();
}
void do0xC7() {
	I8080::_RST(0);
}
void do0xC8() {
	I8080::_RCCC(B001);
}
void do0xC9() {
	I8080::_RET();
}
void do0xCA() {
	I8080::_JCCC(B001);
}
void do0xCB() {
	I8080::_JMP();
}
void do0xCC() {
	I8080::_CCCC(B001);
}
void do0xCD() {
	I8080::_CALL();
}
void do0xCE() {
	I8080::_ACI();
}
void do0xCF() {
	I8080::_RST(1);
}

//D0
void do0xD0() {
	I8080::_RCCC(B010);
}
void do0xD1() {
	I8080::_POP(_RP_DE);
}
void do0xD2() {
	I8080::_JCCC(B010);
}
void do0xD3() {
	I8080::_OUT();
}
void do0xD4() {
	I8080::_CCCC(B010);
}
void do0xD5() {
	I8080::_PUSH(_RP_DE);
}
void do0xD6() {
	I8080::_SUI();
}
void do0xD7() {
	I8080::_RST(2);
}
void do0xD8() {
	I8080::_RCCC(B011);
}
void do0xD9() {
	I8080::_RET();
}
void do0xDA() {
	I8080::_JCCC(B011);
}
void do0xDB() {
	I8080::_IN();
}
void do0xDC() {
	I8080::_CCCC(B011);
}
void do0xDD() {
	I8080::_CALL();
}
void do0xDE() {
	I8080::_SBI();
}
void do0xDF() {
	I8080::_RST(3);
}

//E0
void do0xE0() {
	I8080::_RCCC(B100);
}
void do0xE1() {
	I8080::_POP(_RP_HL);
}
void do0xE2() {
	I8080::_JCCC(B100);
}
void do0xE3() {
	I8080::_XTHL();
}
void do0xE4() {
	I8080::_CCCC(B100);
}
void do0xE5() {
	I8080::_PUSH(_RP_HL);
}
void do0xE6() {
	I8080::_ANI();
}
void do0xE7() {
	I8080::_RST(4);
}
void do0xE8() {
	I8080::_RCCC(B101);
}
void do0xE9() {
	I8080::_PCHL();
}
void do0xEA() {
	I8080::_JCCC(B101);
}
void do0xEB() {
	I8080::_XCHG();
}
void do0xEC() {
	I8080::_CCCC(B101);
}
void do0xED() {
	I8080::_CALL();
}
void do0xEE() {
	I8080::_XRI();
}
void do0xEF() {
	I8080::_RST(5);
}

//F0
void do0xF0() {
	I8080::_RCCC(B110);
}
void do0xF1() {
	I8080::_POP(_RP_AF);
}
void do0xF2() {
	I8080::_JCCC(B110);
}
void do0xF3() {
	I8080::_DI();
}
void do0xF4() {
	I8080::_CCCC(B110);
}
void do0xF5() {
	I8080::_PUSH(_RP_AF);
}
void do0xF6() {
	I8080::_ORI();
}
void do0xF7() {
	I8080::_RST(6);
}
void do0xF8() {
	I8080::_RCCC(B111);
}
void do0xF9() {
	I8080::_SPHL();
}
void do0xFA() {
	I8080::_JCCC(B111);
}
void do0xFB() {
	I8080::_EI();
}
void do0xFC() {
	I8080::_CCCC(B111);
}
void do0xFD() {
	I8080::_CALL();
}
void do0xFE() {
	I8080::_CPI();
}
void do0xFF() {
	I8080::_RST(7);
}

typedef void(*EXEC) ();

// array of function pointers
const EXEC MAP_ARR[] PROGMEM =
{
	//0
	do0x00,
	do0x01,
	do0x02,
	do0x03,
	do0x04,
	do0x05,
	do0x06,
	do0x07,
	do0x08,
	do0x09,
	do0x0A,
	do0x0B,
	do0x0C,
	do0x0D,
	do0x0E,
	do0x0F,

	//1
	do0x10,
	do0x11,
	do0x12,
	do0x13,
	do0x14,
	do0x15,
	do0x16,
	do0x17,
	do0x18,
	do0x19,
	do0x1A,
	do0x1B,
	do0x1C,
	do0x1D,
	do0x1E,
	do0x1F,

	//2
	do0x20,
	do0x21,
	do0x22,
	do0x23,
	do0x24,
	do0x25,
	do0x26,
	do0x27,
	do0x28,
	do0x29,
	do0x2A,
	do0x2B,
	do0x2C,
	do0x2D,
	do0x2E,
	do0x2F,

	//3
	do0x30,
	do0x31,
	do0x32,
	do0x33,
	do0x34,
	do0x35,
	do0x36,
	do0x37,
	do0x38,
	do0x39,
	do0x3A,
	do0x3B,
	do0x3C,
	do0x3D,
	do0x3E,
	do0x3F,

	//4
	do0x40,
	do0x41,
	do0x42,
	do0x43,
	do0x44,
	do0x45,
	do0x46,
	do0x47,
	do0x48,
	do0x49,
	do0x4A,
	do0x4B,
	do0x4C,
	do0x4D,
	do0x4E,
	do0x4F,

	//5
	do0x50,
	do0x51,
	do0x52,
	do0x53,
	do0x54,
	do0x55,
	do0x56,
	do0x57,
	do0x58,
	do0x59,
	do0x5A,
	do0x5B,
	do0x5C,
	do0x5D,
	do0x5E,
	do0x5F,

	//6
	do0x60,
	do0x61,
	do0x62,
	do0x63,
	do0x64,
	do0x65,
	do0x66,
	do0x67,
	do0x68,
	do0x69,
	do0x6A,
	do0x6B,
	do0x6C,
	do0x6D,
	do0x6E,
	do0x6F,

	//7
	do0x70,
	do0x71,
	do0x72,
	do0x73,
	do0x74,
	do0x75,
	do0x76,
	do0x77,
	do0x78,
	do0x79,
	do0x7A,
	do0x7B,
	do0x7C,
	do0x7D,
	do0x7E,
	do0x7F,

	//8
	do0x80,
	do0x81,
	do0x82,
	do0x83,
	do0x84,
	do0x85,
	do0x86,
	do0x87,
	do0x88,
	do0x89,
	do0x8A,
	do0x8B,
	do0x8C,
	do0x8D,
	do0x8E,
	do0x8F,

	//9
	do0x90,
	do0x91,
	do0x92,
	do0x93,
	do0x94,
	do0x95,
	do0x96,
	do0x97,
	do0x98,
	do0x99,
	do0x9A,
	do0x9B,
	do0x9C,
	do0x9D,
	do0x9E,
	do0x9F,

	//A
	do0xA0,
	do0xA1,
	do0xA2,
	do0xA3,
	do0xA4,
	do0xA5,
	do0xA6,
	do0xA7,
	do0xA8,
	do0xA9,
	do0xAA,
	do0xAB,
	do0xAC,
	do0xAD,
	do0xAE,
	do0xAF,

	//B
	do0xB0,
	do0xB1,
	do0xB2,
	do0xB3,
	do0xB4,
	do0xB5,
	do0xB6,
	do0xB7,
	do0xB8,
	do0xB9,
	do0xBA,
	do0xBB,
	do0xBC,
	do0xBD,
	do0xBE,
	do0xBF,

	//C
	do0xC0,
	do0xC1,
	do0xC2,
	do0xC3,
	do0xC4,
	do0xC5,
	do0xC6,
	do0xC7,
	do0xC8,
	do0xC9,
	do0xCA,
	do0xCB,
	do0xCC,
	do0xCD,
	do0xCE,
	do0xCF,

	//D
	do0xD0,
	do0xD1,
	do0xD2,
	do0xD3,
	do0xD4,
	do0xD5,
	do0xD6,
	do0xD7,
	do0xD8,
	do0xD9,
	do0xDA,
	do0xDB,
	do0xDC,
	do0xDD,
	do0xDE,
	do0xDF,

	//E
	do0xE0,
	do0xE1,
	do0xE2,
	do0xE3,
	do0xE4,
	do0xE5,
	do0xE6,
	do0xE7,
	do0xE8,
	do0xE9,
	do0xEA,
	do0xEB,
	do0xEC,
	do0xED,
	do0xEE,
	do0xEF,

	//F
	do0xF0,
	do0xF1,
	do0xF2,
	do0xF3,
	do0xF4,
	do0xF5,
	do0xF6,
	do0xF7,
	do0xF8,
	do0xF9,
	do0xFA,
	do0xFB,
	do0xFC,
	do0xFD,
	do0xFE,
	do0xFF,
};

#endif
