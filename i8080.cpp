/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "CONFIG.h"
#include "CONSOLE.h"
#include "i8080.h"
#include "CMD.h"
#include "MEM.h"
#include "SD.h"
#include "FDD.h"
#include "IO.h"

uint8_t _Regs[8];

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

	uint32_t adr;
	uint8_t port;
	uint8_t dat;
	boolean _EOF;
	boolean error;
	uint8_t hex_count;
	uint8_t hex_len;
	uint8_t hex_type;
	uint8_t hex_crc;
	uint8_t hex_bytes;
	uint16_t len;
	uint16_t count;
	uint8_t crc;
	uint8_t tmp_byte;
	uint16_t tmp_word;

	while (true) {
		do
		{
			CONSOLE::inChar = '\0';
			if (CONSOLE::con_ready()) {
				CONSOLE::inChar = CONSOLE::con_read();
				//Serial.print(uint8_t(inChar),HEX);
				CONSOLE::inChar = CONSOLE::upCase(CONSOLE::inChar);
				if (uint8_t(CONSOLE::inChar) == BS_KEY) {
					//backspace
					if (CONSOLE::mon_ptr > 0) {
						CONSOLE::mon_ptr--;
						CONSOLE::mon_buffer[CONSOLE::mon_ptr] = '\0';
						CONSOLE::write(CONSOLE::inChar);
						CONSOLE::print(" ");
						CONSOLE::write(CONSOLE::inChar);
					}
				}
				else {
					CONSOLE::mon_buffer[CONSOLE::mon_ptr] = CONSOLE::inChar;
					CONSOLE::mon_ptr++;
					CONSOLE::write(CONSOLE::inChar);
				}
			}
		} while ((CONSOLE::inChar != '\r') && (CONSOLE::inChar != '\n') && (CONSOLE::mon_ptr < MON_BUFFER_SIZE));

		CONSOLE::println();
		CONSOLE::mon_ptr = 0;

		CMD::exec();

		CONSOLE::xy(MON_Y, 0);
		CONSOLE::print(">");
		CONSOLE::clrend();
	}
}

void I8080::state() {
	char hex[2];
	CONSOLE::clrlin();
	Serial.print("A:");
	sprintf(hex, "%02X", _Regs[_Reg_A]);
	Serial.print(hex);
	Serial.print("   ");
	Serial.print("B:");
	sprintf(hex, "%02X", _Regs[_Reg_B]);
	Serial.print(hex);
	Serial.print("   ");
	Serial.print("C:");
	sprintf(hex, "%02X", _Regs[_Reg_C]);
	Serial.print(hex);
	Serial.print("   ");
	Serial.print("D:");
	sprintf(hex, "%02X", _Regs[_Reg_D]);
	Serial.print(hex);
	Serial.println("   ");
	CONSOLE::clrlin();
	Serial.print("E:");
	sprintf(hex, "%02X", _Regs[_Reg_E]);
	Serial.print(hex);
	Serial.print("   ");
	Serial.print("H:");
	sprintf(hex, "%02X", _Regs[_Reg_H]);
	Serial.print(hex);
	Serial.print("   ");
	Serial.print("L:");
	sprintf(hex, "%02X", _Regs[_Reg_L]);
	Serial.print(hex);
	Serial.print("   ");
	Serial.print("F: ");
	if (_getFlags_S() == 1) {
		Serial.print("S");
	}
	else {
		Serial.print(" ");
	}
	if (_getFlags_Z() == 1) {
		Serial.print("Z");
	}
	else {
		Serial.print(" ");
	}
	if (_getFlags_A() == 1) {
		Serial.print("A");
	}
	else {
		Serial.print(" ");
	}
	if (_getFlags_P() == 1) {
		Serial.print("P");
	}
	else {
		Serial.print(" ");
	}
	if (_getFlags_C() == 1) {
		Serial.print("C");
	}
	else {
		Serial.print(" ");
	}
	Serial.println("   ");
	CONSOLE::clrlin();
	Serial.print(F("PC:"));
	sprintf(hex, "%02X", highByte(_PC));
	Serial.print(hex);
	sprintf(hex, "%02X", lowByte(_PC));
	Serial.print(hex);
	Serial.print("   ");
	Serial.print(F("SP:"));
	sprintf(hex, "%02X", highByte(_SP));
	Serial.print(hex);
	sprintf(hex, "%02X", lowByte(_SP));
	Serial.println(hex);
	CONSOLE::clrlin();
	Serial.print(F("CMD: "));
	sprintf(hex, "%02X", _IR);
	Serial.print(hex);
	Serial.println("");

	//MMU map
	CONSOLE::clrlin();
	Serial.print(F("MMU: "));
	for (int i = 0; i < MMU_BLOCKS_NUM; i++) {
		Serial.print(MEM::MMU_MAP[i], DEC);
	}
	Serial.println("");
}

uint16_t I8080::pc2a16() {
	uint16_t a16;
	_PC++;
	a16 = MEM::_get(_PC);
	_PC++;
	a16 = a16 + 256 * MEM::_get(_PC);
	return a16;
}

void I8080::pc2sp() {
	_SP--;
	_SP--;
	MEM::_set(_SP, lowByte(_PC));
	MEM::_set(_SP + 1, highByte(_PC));
}

void I8080::_I8080_() {
	_PC++;
}

void I8080::_I8080_HLT() {
	CONFIG::exitFlag = true;
}

//NOP
void I8080::_I8080_NOP() {
	_PC++;
}


void I8080::_I8080_MOV(uint8_t DDD, uint8_t SSS) {
	if (DDD == _Reg_M)
	{
		_TMP = _Regs[SSS];
		MEM::_AB = _rpHL;
		MEM::_DB = _TMP;
		MEM::_WR();
	}
	else
	{
		if (SSS == _Reg_M)
		{
			MEM::_AB = _rpHL;
			MEM::_RD();
			_Regs[DDD] = MEM::_DB;
		}
		else
		{
			_TMP = _Regs[SSS];
			_Regs[DDD] = _TMP;
		}
	}
	_PC++;
}

//MVI
void I8080::_I8080_MVI(uint8_t DDD) {
	_PC++;
	MEM::_AB = _PC;
	if (DDD != _Reg_M)
	{
		MEM::_RD();
		_Regs[DDD] = MEM::_DB;
	}
	else
	{
		MEM::_RD();
		_TMP = MEM::_DB;
		MEM::_AB = _rpHL;
		MEM::_DB = _TMP;
		MEM::_WR();
	}
	_PC++;
}


//LXI
void I8080::_I8080_LXI(uint8_t rp) {
	_PC++;
	MEM::_AB = _PC;
	MEM::_RD();
	switch (rp) {
	case _RP_BC:
		_rC = MEM::_DB;
		break;
	case _RP_DE:
		_rE = MEM::_DB;
		break;
	case _RP_HL:
		_rL = MEM::_DB;
		break;
	case _RP_SP:
		_SP = MEM::_DB;
		break;
	}
	_PC++;
	MEM::_AB = _PC;
	MEM::_RD();
	switch (rp) {
	case _RP_BC:
		_rB = MEM::_DB;
		break;
	case _RP_DE:
		_rD = MEM::_DB;
		break;
	case _RP_HL:
		_rH = MEM::_DB;
		break;
	case _RP_SP:
		_SP = _SP + 256 * MEM::_DB;
		break;
	}
	_PC++;
}

//LDA
void I8080::_I8080_LDA() {
	_PC++;
	MEM::_AB = _PC;
	MEM::_RD();
	_Z = MEM::_DB;
	_PC++;
	MEM::_AB = _PC;
	MEM::_RD();
	_W = MEM::_DB;
	MEM::_AB = _rpWZ;
	MEM::_RD();
	_rA = MEM::_DB;
	_PC++;
}

//STA
void I8080::_I8080_STA() {
	_PC++;
	MEM::_AB = _PC;
	MEM::_RD();
	_Z = MEM::_DB;
	_PC++;
	MEM::_AB = _PC;
	MEM::_RD();
	_W = MEM::_DB;
	MEM::_AB = _rpWZ;
	MEM::_DB = _rA;
	MEM::_WR();
	_PC++;
}


//LHLD
void I8080::_I8080_LHLD()
{
	_PC++;
	MEM::_AB = _PC;
	MEM::_RD();
	_Z = MEM::_DB;
	_PC++;
	MEM::_AB = _PC;
	MEM::_RD();
	_W = MEM::_DB;
	MEM::_AB = _rpWZ;
	MEM::_RD();
	_rL = MEM::_DB;
	_Z++;
	if (_Z == 0) {
		_W++;
	}
	MEM::_AB = _rpWZ;
	MEM::_RD();
	_rH = MEM::_DB;
	_PC++;
}

//SHLD
void I8080::_I8080_SHLD() {
	_PC++;
	MEM::_AB = _PC;
	MEM::_RD();
	_Z = MEM::_DB;
	_PC++;
	MEM::_AB = _PC;
	MEM::_RD();
	_W = MEM::_DB;
	MEM::_AB = _rpWZ;
	MEM::_DB = _rL;
	MEM::_WR();
	_Z++;
	if (_Z == 0) {
		_W++;
	}
	MEM::_AB = _rpWZ;
	MEM::_DB = _rH;
	MEM::_WR();
	_PC++;
}

//LDAX
void I8080::_I8080_LDAX(uint8_t rp) {
	switch (rp) {
	case _RP_BC:
		MEM::_AB = _rpBC;
		break;
	case _RP_DE:
		MEM::_AB = _rpDE;
		break;
	}
	MEM::_RD();
	_rA = MEM::_DB;
	_PC++;
}

//STAX
void I8080::_I8080_STAX(uint8_t rp) {
	switch (rp) {
	case _RP_BC:
		MEM::_AB = _rpBC;
		break;
	case _RP_DE:
		MEM::_AB = _rpDE;
		break;
	}
	MEM::_DB = _rA;
	MEM::_WR();
	_PC++;
}

//XCHG
void I8080::_I8080_XCHG() {
	_rD = _rD ^ _rH;
	_rH = _rD ^ _rH;
	_rD = _rD ^ _rH;
	_rE = _rE ^ _rL;
	_rL = _rE ^ _rL;
	_rE = _rE ^ _rL;
	_PC++;
}

//ADD
void I8080::_I8080_ADD(uint8_t SSS) {
	if (SSS != _Reg_M) {
		_TMP = _Regs[SSS];
		_ACT = _rA;
	}
	else {
		_ACT = _rA;
		MEM::_AB = _rpHL;
		MEM::_RD();
		_TMP = MEM::_DB;
	}
	if ((_ACT & B1111) + (_TMP & B1111) > B1111) {
		_setFlags_A(1);
	}
	else {
		_setFlags_A(0);
	}
	_rA = _ACT + _TMP;
	if ((_rA < _ACT) && (_rA < _TMP)) {
		_setFlags_C(1);
	}
	else {
		_setFlags_C(0);
	}
	_FLAGS(_rA);
	_PC++;
}

//ADI
void I8080::_I8080_ADI() {
	_PC++;
	_ACT = _rA;
	MEM::_AB = _PC;
	MEM::_RD();
	_TMP = MEM::_DB;
	if ((_ACT & B1111) + (_TMP & B1111) > B1111) {
		_setFlags_A(1);
	}
	else {
		_setFlags_A(0);
	}
	_rA = _ACT + _TMP;
	if ((_rA < _ACT) && (_rA < _TMP)) {
		_setFlags_C(1);
	}
	else {
		_setFlags_C(0);
	}
	_FLAGS(_rA);
	_PC++;
}


//ADC
void I8080::_I8080_ADC(uint8_t SSS) {
	if (SSS != _Reg_M) {
		_TMP = _Regs[SSS];
		_ACT = _rA;
	}
	else {
		_ACT = _rA;
		MEM::_AB = _rpHL;
		MEM::_RD();
		_TMP = MEM::_DB;
	}
	if ((_ACT & B1111) + (_TMP & B1111) > B1111) {
		_setFlags_A(1);
	}
	else {
		_setFlags_A(0);
	}
	_TMP = _TMP + _getFlags_C();
	_rA = _ACT + _TMP;
	if ((_rA < _ACT) && (_rA < _TMP)) {
		_setFlags_C(1);
	}
	else {
		_setFlags_C(0);
	}
	_FLAGS(_rA);
	_PC++;
}

//ACI
void I8080::_I8080_ACI() {
	_PC++;
	_ACT = _rA;
	MEM::_AB = _PC;
	MEM::_RD();
	_TMP = MEM::_DB;
	if ((_ACT & B1111) + (_TMP & B1111) > B1111) {
		_setFlags_A(1);
	}
	else {
		_setFlags_A(0);
	}
	_TMP = _TMP + _getFlags_C();
	_rA = _ACT + _TMP;
	if ((_rA < _ACT) && (_rA < _TMP)) {
		_setFlags_C(1);
	}
	else {
		_setFlags_C(0);
	}
	_FLAGS(_rA);
	_PC++;
}

//SUB
void I8080::_I8080_SUB(uint8_t SSS) {
	if (SSS != _Reg_M) {
		_TMP = _Regs[SSS];
		_ACT = _rA;
	}
	else {
		_ACT = _rA;
		MEM::_AB = _rpHL;
		MEM::_RD();
		_TMP = MEM::_DB;
	}
	_TMP = _TMP ^ 0xFF;
	_rA = _ACT + _TMP + 1;
	if (uint16_t(_ACT + _TMP + 1) > 0xFF) {
		_setFlags_C(0);
	}
	else
	{
		_setFlags_C(1);
	}
	_FLAGS(_rA);
	if ((_ACT & 0xF) + (_rA & 0xF) > 0xF) {
		_setFlags_A(1);
	}
	else {
		_setFlags_A(0);
	}
	_PC++;
}

//SUI
void I8080::_I8080_SUI() {
	_PC++;
	_ACT = _rA;
	MEM::_AB = _PC;
	MEM::_RD();
	_TMP = MEM::_DB;
	_TMP = _TMP ^ 0xFF;
	_rA = _ACT + _TMP + 1;
	if (uint16_t(_ACT + _TMP + 1) > 0xFF) {
		_setFlags_C(0);
	}
	else
	{
		_setFlags_C(1);
	}
	_FLAGS(_rA);
	if ((_ACT & 0xF) + (_rA & 0xF) > 0xF) {
		_setFlags_A(1);
	}
	else {
		_setFlags_A(0);
	}
	_PC++;
	MEM::_AB = _PC;
}

//SBB
void I8080::_I8080_SBB(uint8_t SSS) {
	if (SSS != _Reg_M) {
		_TMP = _Regs[SSS];
		_ACT = _rA;
	}
	else {
		_ACT = _rA;
		MEM::_AB = _rpHL;
		MEM::_RD();
		_TMP = MEM::_DB;
	}
	_TMP = _TMP + _getFlags_C();
	_TMP = _TMP ^ 0xFF;
	_rA = _ACT + _TMP + 1;
	if (uint16_t(_ACT + _TMP + 1) > 0xFF) {
		_setFlags_C(0);
	}
	else
	{
		_setFlags_C(1);
	}
	_FLAGS(_rA);
	if ((_ACT & 0xF) + (_rA & 0xF) > 0xF) {
		_setFlags_A(1);
	}
	else {
		_setFlags_A(0);
	}
	_PC++;
}

void I8080::_I8080_SBI() {
	_PC++;
	_ACT = _rA;
	MEM::_AB = _PC;
	MEM::_RD();
	_TMP = MEM::_DB;
	_TMP = _TMP + _getFlags_C();
	_TMP = _TMP ^ 0xFF;
	_rA = _ACT + _TMP + 1;
	if (uint16_t(_ACT + _TMP + 1) > 0xFF) {
		_setFlags_C(0);
	}
	else
	{
		_setFlags_C(1);
	}
	_FLAGS(_rA);
	if ((_ACT & 0xF) + (_rA & 0xF) > 0xF) {
		_setFlags_A(1);
	}
	else {
		_setFlags_A(0);
	}
	_PC++;
	MEM::_AB = _PC;
}

//The Auxiliary Carry bit will be affected by all addition,
//subtraction, increment, decrement, and compare
//instructions.

//INR
void I8080::_I8080_INR(uint8_t DDD) {
	if (DDD == _Reg_M)
	{
		MEM::_AB = _rpHL;
		MEM::_RD();
		_TMP = MEM::_DB;
	}
	else {
		_TMP = _Regs[DDD];
	}
	_ALU = _TMP + 1;
	if (DDD == _Reg_M)
	{
		MEM::_DB = _ALU;
		MEM::_AB = _rpHL;
		MEM::_WR();
	}
	else {
		_Regs[DDD] = _ALU;
	}
	_FLAGS(_ALU);
	_setFlags_C(0);
	if ((_ALU & 0x0F) == 0x00) {
		_setFlags_A(1);
	}
	else {
		_setFlags_A(0);
	}
	_PC++;
}

//DCR
void I8080::_I8080_DCR(uint8_t DDD) {
	if (DDD == _Reg_M)
	{
		MEM::_AB = _rpHL;
		MEM::_RD();
		_TMP = MEM::_DB;
	}
	else {
		_TMP = _Regs[DDD];
	}
	_ALU = _TMP - 1;
	if (DDD == _Reg_M)
	{
		MEM::_DB = _ALU;
		MEM::_AB = _rpHL;
		MEM::_WR();
	}
	else {
		_Regs[DDD] = _ALU;
	}
	_FLAGS(_ALU);
	_setFlags_C(0);
	if (!((_ALU & 0x0F) == 0x0F)) {
		_setFlags_A(1);
	}
	else {
		_setFlags_A(0);
	}
	_PC++;
}

//INX
void I8080::_I8080_INX(uint8_t rp) {
	switch (rp) {
	case _RP_BC:
		_rC++;
		if (_rC == 0) {
			_rB++;
		}
		break;
	case _RP_DE:
		_rE++;
		if (_rE == 0) {
			_rD++;
		}
		break;
	case _RP_HL:
		_rL++;
		if (_rL == 0) {
			_rH++;
		}
		break;
	case _RP_SP:
		_SP++;
		break;
	}
	_PC++;
}

//DCX
void I8080::_I8080_DCX(uint8_t rp) {
	switch (rp) {
	case _RP_BC:
		_rC--;
		if (_rC == 0xFF) {
			_rB--;
		}
		break;
	case _RP_DE:
		_rE--;
		if (_rE == 0xFF) {
			_rD--;
		}
		break;
	case _RP_HL:
		_rL--;
		if (_rL == 0xFF) {
			_rH--;
		}
		break;
	case _RP_SP:
		_SP--;
		break;
	}
	_PC++;
}

//DAD
void I8080::_I8080_DAD(uint8_t rp) {
	uint16_t d16_1;
	uint16_t d16_2;
	uint16_t d16_3;
	d16_1 = _rH * 256 + _rL;
	switch (rp) {
	case _RP_BC:
		d16_2 = _rB * 256 + _rC;
		break;
	case _RP_DE:
		d16_2 = _rD * 256 + _rE;
		break;
	case _RP_HL:
		d16_2 = _rH * 256 + _rL;
		break;
	case _RP_SP:
		d16_2 = _SP;
		break;
	}
	d16_3 = d16_1 + d16_2;
	_rH = highByte(d16_3);
	_rL = lowByte(d16_3);
	if ((d16_3 < d16_1) || (d16_3 < d16_2)) {
		_setFlags_C(1);
	}
	else {
		_setFlags_C(0);
	}
	_PC++;
}

//DAA
void I8080::_I8080_DAA() {
	uint8_t d8;
	d8 = 0;
	if (((_rA & 0x0F) > 9) || (_getFlags_A() == 1))
	{
		if ((_rA & 0x0F) > 9) {
			_setFlags_A(1);
		}
		else
		{
			_setFlags_A(0);
		}
		d8 = 6;
	}
	if ((((_rA & 0xF0) >> 4) > 9) || (_getFlags_C() == 1) || ((((_rA & 0xF0) >> 4) >= 9) && ((_rA & 0x0F) > 9)))
	{
		if ((((_rA & 0xF0) >> 4) > 9) || ((((_rA & 0xF0) >> 4) >= 9) && ((_rA & 0x0F) > 9))) {
			_setFlags_C(1);
		}
		d8 = d8 | 0x60;
	}
	_rA = _rA + d8;
	//SZP flags
	_rF = (_rF & ZP_RESET) | (pgm_read_byte_near(SZP_table + _rA) & B01111111);
	_PC++;
}

//ANA
void I8080::_I8080_ANA(uint8_t SSS) {
	if (SSS == _Reg_M)
	{
		_ACT = _rA;
		MEM::_AB = _rpHL;
		MEM::_RD();
		_TMP = MEM::_DB;
	}
	else
	{
		_TMP = _Regs[SSS];
		_ACT = _rA;
	}
	//8080 - CY = 0, AC = bits 3 _rA OR d8
	_setFlags_C(0);
	if (((_ACT & B1000) | (_TMP & B1000)) != 0) {
		_setFlags_A(1);
	}
	else
	{
		_setFlags_A(0);
	}
	//8085 - CY = 0, AC = 1
	_rA = _ACT & _TMP;
	_FLAGS(_rA);
	_PC++;
}

//ANI
void I8080::_I8080_ANI() {
	_PC++;
	_ACT = _rA;
	MEM::_AB = _PC;
	MEM::_RD();
	_TMP = MEM::_DB;
	//8080 - CY = 0, AC = bits 3 _rA OR d8
	_setFlags_C(0);
	if (((_ACT & B1000) | (_TMP & B1000)) != 0) {
		_setFlags_A(1);
	}
	else
	{
		_setFlags_A(0);
	}
	_rA = _ACT & _TMP;
	_FLAGS(_rA);
	_PC++;
}

//ORA
void I8080::_I8080_ORA(uint8_t SSS) {
	if (SSS == _Reg_M)
	{
		_ACT = _rA;
		MEM::_AB = _rpHL;
		MEM::_RD();
		_TMP = MEM::_DB;
	}
	else
	{
		_TMP = _Regs[SSS];
		_ACT = _rA;
	}
	_rA = _ACT | _TMP;
	_FLAGS(_rA);
	_setFlags_C(0);
	_setFlags_A(0);
	_PC++;
}

//ORI
void I8080::_I8080_ORI() {
	_PC++;
	_ACT = _rA;
	MEM::_AB = _PC;
	MEM::_RD();
	_TMP = MEM::_DB;
	_rA = _ACT | _TMP;
	_FLAGS(_rA);
	_setFlags_C(0);
	_setFlags_A(0);
	_PC++;
}

//XRA
void I8080::_I8080_XRA(uint8_t SSS) {
	if (SSS == _Reg_M)
	{
		_ACT = _rA;
		MEM::_AB = _rpHL;
		MEM::_RD();
		_TMP = MEM::_DB;
	}
	else
	{
		_TMP = _Regs[SSS];
		_ACT = _rA;
	}
	_rA = _ACT ^ _TMP;
	_FLAGS(_rA);
	_setFlags_C(0);
	_setFlags_A(0);
	_PC++;
}

//XRI
void I8080::_I8080_XRI() {
	_PC++;
	_ACT = _rA;
	MEM::_AB = _PC;
	MEM::_RD();
	_TMP = MEM::_DB;
	_rA = _ACT ^ _TMP;
	_FLAGS(_rA);
	_setFlags_C(0);
	_setFlags_A(0);
	_PC++;
}

//CMP
void I8080::_I8080_CMP(uint8_t SSS) {
	if (SSS != _Reg_M) {
		_ACT = _rA;
		_TMP = _Regs[SSS];
	}
	else {
		_ACT = _rA;
		MEM::_AB = _rpHL;
		MEM::_RD();
		_TMP = MEM::_DB;
	}
	if (_rA < _TMP) {
		_setFlags_C(1);
	}
	else {
		_setFlags_C(0);
	}
	_TMP = _TMP ^ 0xFF;
	if ((_ACT & 0xF) + (_TMP & 0xF) > 0xF) {
		_setFlags_A(1);
	}
	else {
		_setFlags_A(0);
	}
	_TMP = _TMP + 1;
	_FLAGS(lowByte(_ACT + _TMP));
	_PC++;
}

//CPI
void I8080::_I8080_CPI() {
	_ACT = _rA;
	_PC++;
	MEM::_AB = _PC;
	MEM::_RD();
	_TMP = MEM::_DB;
	if (_ACT < _TMP) {
		_setFlags_C(1);
	}
	else {
		_setFlags_C(0);
	}
	_TMP = _TMP ^ 0xFF;
	if ((_ACT & 0xF) + (_TMP & 0xF) > 0xF) {
		_setFlags_A(1);
	}
	else {
		_setFlags_A(0);
	}
	_TMP = _TMP + 1;
	_FLAGS(lowByte(_ACT + _TMP));
	_PC++;
}

//RRC
void I8080::_I8080_RRC() {
	_ALU = _rA;
	_setFlags_C(bitRead(_ALU, 0));
	_rA = _ALU >> 1;
	bitWrite(_rA, 7, _getFlags_C());
	_PC++;
}

//RLC
void I8080::_I8080_RLC() {
	_ALU = _rA;
	_setFlags_C(bitRead(_ALU, 7));
	_rA = _ALU << 1;
	bitWrite(_rA, 0, _getFlags_C());
	_PC++;
}

//RAL
void I8080::_I8080_RAL() {
	_ALU = _rA;
	_rA = ((_ALU << 1) & B11111110) | _getFlags_C();
	_setFlags_C(bitRead(_ALU, 7));
	_PC++;
}

//RAR
void I8080::_I8080_RAR() {
	_ALU = _rA;
	_rA = ((_ALU >> 1) & B01111111) | (_getFlags_C() << 7);
	_setFlags_C(bitRead(_ALU, 0));
	_PC++;
}

//CMA
void I8080::_I8080_CMA() {
	_rA = ~_rA;
	_PC++;
}

//CMC
void I8080::_I8080_CMC() {
	if (_getFlags_C() != 0)
	{
		_setFlags_C(0);
	}
	else {
		_setFlags_C(1);
	}
	_PC++;
}

//STC
void I8080::_I8080_STC() {
	_setFlags_C(1);
	_PC++;
}

//JMP
void I8080::_I8080_JMP() {
	_PC++;
	MEM::_AB = _PC;
	MEM::_RD();
	_PC++;
	_Z = MEM::_DB;
	MEM::_AB = _PC;
	MEM::_RD();
	_PC++;
	_W = MEM::_DB;
	_PC = _rpWZ;
}

//Jccc
void I8080::_I8080_JCCC(uint8_t CCC)
{
	boolean COND = false;
	switch (CCC) {
	case B000: if (_getFlags_Z() == 0) {
		COND = true;  //NZ
	}
			   break;
	case B001: if (_getFlags_Z() == 1) {
		COND = true;  //Z
	}
			   break;
	case B010: if (_getFlags_C() == 0) {
		COND = true;  //NC
	}
			   break;
	case B011: if (_getFlags_C() == 1) {
		COND = true;  //C
	}
			   break;
	case B100: if (_getFlags_P() == 0) {
		COND = true;  //PO
	}
			   break;
	case B101: if (_getFlags_P() == 1) {
		COND = true;  //PE
	}
			   break;
	case B110: if (_getFlags_S() == 0) {
		COND = true;  //P
	}
			   break;
	case B111: if (_getFlags_S() == 1) {
		COND = true;  //M
	}
			   break;
	}
	_PC++;
	MEM::_AB = _PC;
	MEM::_RD();
	_PC++;
	_Z = MEM::_DB;
	MEM::_AB = _PC;
	MEM::_RD();
	_PC++;
	_W = MEM::_DB;
	if (COND) {
		_PC = _rpWZ;
	}
}

//CALL
void I8080::_I8080_CALL() {
	_SP--;
	_PC++;
	MEM::_AB = _PC;
	MEM::_RD();
	_PC++;
	_Z = MEM::_DB;
	MEM::_AB = _PC;
	MEM::_RD();
	_PC++;
	_W = MEM::_DB;
	MEM::_AB = _SP;
	MEM::_DB = highByte(_PC);
	MEM::_WR();
	_SP--;
	MEM::_AB = _SP;
	MEM::_DB = lowByte(_PC);
	MEM::_WR();
	_PC = _rpWZ;
}

//Cccc
void I8080::_I8080_CCCC(uint8_t CCC) {
	boolean COND = false;
	switch (CCC) {
	case B000: if (_getFlags_Z() == 0) {
		COND = true;  //NZ
	}
			   break;
	case B001: if (_getFlags_Z() == 1) {
		COND = true;  //Z
	}
			   break;
	case B010: if (_getFlags_C() == 0) {
		COND = true;  //NC
	}
			   break;
	case B011: if (_getFlags_C() == 1) {
		COND = true;  //C
	}
			   break;
	case B100: if (_getFlags_P() == 0) {
		COND = true;  //PO
	}
			   break;
	case B101: if (_getFlags_P() == 1) {
		COND = true;  //PE
	}
			   break;
	case B110: if (_getFlags_S() == 0) {
		COND = true;  //P
	}
			   break;
	case B111: if (_getFlags_S() == 1) {
		COND = true;  //M
	}
			   break;
	}
	if (COND) {
		_SP--;
	};
	_PC++;
	MEM::_AB = _PC;
	MEM::_RD();
	_PC++;
	_Z = MEM::_DB;
	MEM::_AB = _PC;
	MEM::_RD();
	_PC++;
	_W = MEM::_DB;
	if (COND) {
		MEM::_AB = _SP;
		MEM::_DB = highByte(_PC);
		MEM::_WR();
		_SP--;
		MEM::_AB = _SP;
		MEM::_DB = lowByte(_PC);
		MEM::_WR();
		_PC = _rpWZ;
	}
}

//RET
void I8080::_I8080_RET()
{
	MEM::_AB = _SP;
	MEM::_RD();
	_SP++;
	_Z = MEM::_DB;
	MEM::_AB = _SP;
	MEM::_RD();
	_SP++;
	_W = MEM::_DB;
	_PC = _rpWZ;
}

//Rccc
void I8080::_I8080_RCCC(uint8_t CCC)
{
	boolean COND = false;
	switch (CCC) {
	case B000: if (_getFlags_Z() == 0) {
		COND = true;  //NZ
	}
			   break;
	case B001: if (_getFlags_Z() == 1) {
		COND = true;  //Z
	}
			   break;
	case B010: if (_getFlags_C() == 0) {
		COND = true;  //NC
	}
			   break;
	case B011: if (_getFlags_C() == 1) {
		COND = true;  //C
	}
			   break;
	case B100: if (_getFlags_P() == 0) {
		COND = true;  //PO
	}
			   break;
	case B101: if (_getFlags_P() == 1) {
		COND = true;  //PE
	}
			   break;
	case B110: if (_getFlags_S() == 0) {
		COND = true;  //P
	}
			   break;
	case B111: if (_getFlags_S() == 1) {
		COND = true;  //M
	}
			   break;
	}
	if (COND) {
		MEM::_AB = _SP;
		MEM::_RD();
		_SP++;
		_Z = MEM::_DB;
		MEM::_AB = _SP;
		MEM::_RD();
		_SP++;
		_W = MEM::_DB;
		_PC = _rpWZ;
	}
	else {
		_PC++;
	}
}

//RST
void I8080::_I8080_RST(uint8_t n) {
	_PC++;
	pc2sp();
	_PC = n * 8;
}


//PCHL
void I8080::_I8080_PCHL() {
	_PC = _rpHL;
	MEM::_AB = _PC;
}

//XTHL
void I8080::_I8080_XTHL() {
	MEM::_AB = _SP;
	MEM::_RD();
	_SP++;
	_Z = MEM::_DB;
	MEM::_AB = _SP;
	MEM::_RD();
	_W = MEM::_DB;
	MEM::_AB = _SP;
	MEM::_DB = _rH;
	MEM::_WR();
	_SP--;
	MEM::_AB = _SP;
	MEM::_DB = _rL;
	MEM::_WR();
	_rL = _Z;
	_rH = _W;
	_PC++;
}

//SPHL
void I8080::_I8080_SPHL()
{
	_SP = _rpHL;
	_PC++;
}

//PUSH
void I8080::_I8080_PUSH(uint8_t rp) {
	_SP--;
	MEM::_AB = _SP;
	_SP--;
	switch (rp) {
	case _RP_BC:
		MEM::_DB = _rB;
		break;
	case _RP_DE:
		MEM::_DB = _rD;
		break;
	case _RP_AF:
		MEM::_DB = _rA;
		break;
	case _RP_HL:
		MEM::_DB = _rH;
		break;
	}
	MEM::_WR();
	MEM::_AB = _SP;
	switch (rp) {
	case _RP_BC:
		MEM::_DB = _rC;
		break;
	case _RP_DE:
		MEM::_DB = _rE;
		break;
	case _RP_AF:
		MEM::_DB = _rF;
		break;
	case _RP_HL:
		MEM::_DB = _rL;
		break;
	}
	MEM::_WR();
	_PC++;
}

//POP
void I8080::_I8080_POP(uint8_t rp) {
	MEM::_AB = _SP;
	MEM::_RD();
	_SP++;
	switch (rp) {
	case _RP_BC:
		_rC = MEM::_DB;
		break;
	case _RP_DE:
		_rE = MEM::_DB;
		break;
	case _RP_AF:
		_rF = MEM::_DB;
		break;
	case _RP_HL:
		_rL = MEM::_DB;
		break;
	}
	MEM::_AB = _SP;
	MEM::_RD();
	_SP++;
	switch (rp) {
	case _RP_BC:
		_rB = MEM::_DB;
		break;
	case _RP_DE:
		_rD = MEM::_DB;
		break;
	case _RP_AF:
		_rA = MEM::_DB;
		break;
	case _RP_HL:
		_rH = MEM::_DB;
		break;
	}
	_PC++;
}

//IN
void I8080::_I8080_IN() {
	_PC++;
	MEM::_AB = _PC;
	MEM::_RD();
	_Z = MEM::_DB;
	_W = MEM::_DB;
	MEM::_AB = _rpWZ;
	IO::_INPORT();
	_rA = MEM::_DB;
	_PC++;
}

//OUT
void I8080::_I8080_OUT() {
	_PC++;
	MEM::_AB = _PC;
	MEM::_RD();
	_Z = MEM::_DB;
	_W = MEM::_DB;
	MEM::_AB = _rpWZ;
	MEM::_DB = _rA;
	IO::_OUTPORT();
	_PC++;
}

//EI
void I8080::_I8080_EI() {
	INTE = true;
	_PC++;
}

//DI
void I8080::_I8080_DI() {
	INTE = false;
	_PC++;
}
