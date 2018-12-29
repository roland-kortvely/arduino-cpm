/*  CPM4NANO - i8080 & CP/M emulator for Arduino Nano 3.0
    Copyright (C) 2017 - Alexey V. Voronin @ FoxyLab
    Email:    support@foxylab.com
    Website:  https://acdc.foxylab.com
*/

//TO DO
//INR  DCR - AC

extern uint8_t _getMEM(uint16_t adr);
extern void _setMEM(uint16_t adr, uint8_t dat);
extern boolean con_ready();
extern char con_read();

//registers
volatile uint8_t _W;// W register
volatile uint8_t _Z;// Z register
volatile uint8_t _ACT;// ACT register
volatile uint8_t _TMP;// TMP register
volatile uint8_t _ALU;// ALU
volatile uint16_t _PC; //program counter
volatile uint16_t _SP; //stack pointer
volatile uint8_t _IR; //instruction register

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

uint8_t _Regs[8];
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

uint8_t SZP_RESET = B00111011;
uint8_t ZP_RESET = B10111011;

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

uint16_t breakpoint = 0xFFFF;
boolean breakpointFlag = false;
bool INTE;
boolean DEBUG;//debug mode flag

uint16_t pc2a16() {
  uint16_t a16;
  _PC++;
  a16 = _getMEM(_PC);
  _PC++;
  a16 = a16 + 256 * _getMEM(_PC);
  return a16;
}

void pc2sp() {
  _SP--;
  _SP--;
  _setMEM(_SP, lowByte(_PC));
  _setMEM(_SP + 1, highByte(_PC));
}

void state() {
  char hex[2];
  clrlin();
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
  clrlin();
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
  if (_getFlags_S()==1) {
    Serial.print("S");
  }
  else {
    Serial.print(" ");
  }
  if (_getFlags_Z()==1) {
    Serial.print("Z");
  }
  else {
    Serial.print(" ");
  }
  if (_getFlags_A()==1) {
    Serial.print("A");
  }
  else {
    Serial.print(" ");
  }
  if (_getFlags_P()==1) {
    Serial.print("P");
  }
  else {
    Serial.print(" ");
  }
  if (_getFlags_C()==1) {
    Serial.print("C");
  }
  else {
    Serial.print(" ");
  }
  Serial.println("   ");
  clrlin();
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
  clrlin();
  Serial.print(F("CMD: "));
  sprintf(hex, "%02X", _IR);
  Serial.print(hex);
  Serial.println("");
  //MMU map
  clrlin();
  Serial.print(F("MMU: "));
  for(int i=0;i<MMU_BLOCKS_NUM;i++) {
    Serial.print(MMU_MAP[i],DEC);
  }
  Serial.println("");
}

void _I8080_() {
  _PC++;
}

void _I8080_HLT() {
  exitFlag = true;
}

//NOP
void _I8080_NOP() {
  _PC++;
}


void _I8080_MOV(uint8_t DDD, uint8_t SSS) {
  if (DDD == _Reg_M)
  {
    _TMP = _Regs[SSS];
    _AB = _rpHL;
    _DB = _TMP;
    _WRMEM();
  }
  else
  {
    if (SSS == _Reg_M)
    {
      _AB = _rpHL;
      _RDMEM();
      _Regs[DDD] = _DB;
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
void _I8080_MVI(uint8_t DDD) {
  _PC++;
  _AB = _PC;
  if (DDD != _Reg_M)
  {
    _RDMEM();
    _Regs[DDD] = _DB;
  }
  else
  {
    _RDMEM();
    _TMP = _DB;
    _AB = _rpHL;
    _DB = _TMP;
    _WRMEM();
  }
  _PC++;
}


//LXI
void _I8080_LXI(uint8_t rp) {
  _PC++;
  _AB = _PC;
  _RDMEM();
  switch (rp) {
    case _RP_BC:
      _rC = _DB;
      break;
    case _RP_DE:
      _rE = _DB;
      break;
    case _RP_HL:
      _rL = _DB;
      break;
    case _RP_SP:
      _SP = _DB;
      break;
  }
  _PC++;
  _AB = _PC;
  _RDMEM();
  switch (rp) {
    case _RP_BC:
      _rB = _DB;
      break;
    case _RP_DE:
      _rD = _DB;
      break;
    case _RP_HL:
      _rH = _DB;
      break;
    case _RP_SP:
      _SP = _SP + 256 * _DB;
      break;
  }
  _PC++;
}

//LDA
void _I8080_LDA() {
  _PC++;
  _AB = _PC;
  _RDMEM();
  _Z = _DB;
  _PC++;
  _AB = _PC;
  _RDMEM();
  _W = _DB;
  _AB = _rpWZ;
  _RDMEM();
  _rA = _DB;
  _PC++;
}

//STA
void _I8080_STA() {
  _PC++;
  _AB = _PC;
  _RDMEM();
  _Z = _DB;
  _PC++;
  _AB = _PC;
  _RDMEM();
  _W = _DB;
  _AB = _rpWZ;
  _DB = _rA;
  _WRMEM();
  _PC++;
}


//LHLD
void _I8080_LHLD()
{
  _PC++;
  _AB = _PC;
  _RDMEM();
  _Z = _DB;
  _PC++;
  _AB = _PC;
  _RDMEM();
  _W = _DB;
  _AB = _rpWZ;
  _RDMEM();
  _rL = _DB;
  _Z++;
  if (_Z == 0) {
    _W++;
  }
  _AB = _rpWZ;
  _RDMEM();
  _rH = _DB;
  _PC++;
}

//SHLD
void _I8080_SHLD() {
  _PC++;
  _AB = _PC;
  _RDMEM();
  _Z = _DB;
  _PC++;
  _AB = _PC;
  _RDMEM();
  _W = _DB;
  _AB = _rpWZ;
  _DB = _rL;
  _WRMEM();
  _Z++;
  if (_Z == 0) {
    _W++;
  }
  _AB = _rpWZ;
  _DB = _rH;
  _WRMEM();
  _PC++;
}

//LDAX
void _I8080_LDAX(uint8_t rp) {
  switch (rp) {
    case _RP_BC:
      _AB = _rpBC;
      break;
    case _RP_DE:
      _AB = _rpDE;
      break;
  }
  _RDMEM();
  _rA = _DB;
  _PC++;
}

//STAX
void _I8080_STAX(uint8_t rp) {
  switch (rp) {
    case _RP_BC:
      _AB = _rpBC;
      break;
    case _RP_DE:
      _AB = _rpDE;
      break;
  }
  _DB = _rA;
  _WRMEM();
  _PC++;
}

//XCHG
void _I8080_XCHG() {
  _rD = _rD ^ _rH;
  _rH = _rD ^ _rH;
  _rD = _rD ^ _rH;
  _rE = _rE ^ _rL;
  _rL = _rE ^ _rL;
  _rE = _rE ^ _rL;
  _PC++;
}

//ADD
void _I8080_ADD(uint8_t SSS) {
  if (SSS != _Reg_M) {
    _TMP = _Regs[SSS];
    _ACT = _rA;
  }
  else {
    _ACT = _rA;
    _AB = _rpHL;
    _RDMEM();
    _TMP = _DB;
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
void _I8080_ADI() {
  _PC++;
  _ACT = _rA;
  _AB = _PC;
  _RDMEM();
  _TMP = _DB;
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
void _I8080_ADC(uint8_t SSS) {
  if (SSS != _Reg_M) {
    _TMP = _Regs[SSS];
    _ACT = _rA;
  }
  else {
    _ACT = _rA;
    _AB = _rpHL;
    _RDMEM();
    _TMP = _DB;
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
void _I8080_ACI() {
  _PC++;
  _ACT = _rA;
  _AB = _PC;
  _RDMEM();
  _TMP = _DB;
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
void _I8080_SUB(uint8_t SSS) {
  if (SSS != _Reg_M) {
    _TMP = _Regs[SSS];
    _ACT = _rA;
  }
  else {
    _ACT = _rA;
    _AB = _rpHL;
    _RDMEM();
    _TMP = _DB;
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
void _I8080_SUI() {
  _PC++;
  _ACT = _rA;
  _AB = _PC;
  _RDMEM();
  _TMP = _DB;
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
  _AB = _PC;
}

//SBB
void _I8080_SBB(uint8_t SSS) {
  if (SSS != _Reg_M) {
    _TMP = _Regs[SSS];
    _ACT = _rA;
  }
  else {
    _ACT = _rA;
    _AB = _rpHL;
    _RDMEM();
    _TMP = _DB;
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

void _I8080_SBI() {
  _PC++;
  _ACT = _rA;
  _AB = _PC;
  _RDMEM();
  _TMP = _DB;
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
  _AB = _PC;
}

//The Auxiliary Carry bit will be affected by all addition,
//subtraction, increment, decrement, and compare
//instructions.

//INR
void _I8080_INR(uint8_t DDD) {
  if (DDD == _Reg_M)
  {
    _AB = _rpHL;
    _RDMEM();
    _TMP = _DB;
  }
  else {
    _TMP = _Regs[DDD];
  }
  _ALU = _TMP + 1;
  if (DDD == _Reg_M)
  {
    _DB = _ALU;
    _AB = _rpHL;
    _WRMEM();
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
void _I8080_DCR(uint8_t DDD) {
  if (DDD == _Reg_M)
  {
    _AB = _rpHL;
    _RDMEM();
    _TMP = _DB;
  }
  else {
    _TMP = _Regs[DDD];
  }
  _ALU = _TMP - 1;
  if (DDD == _Reg_M)
  {
    _DB = _ALU;
    _AB = _rpHL;
    _WRMEM();
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
void _I8080_INX(uint8_t rp) {
  switch (rp)  {
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
void _I8080_DCX(uint8_t rp) {
  switch (rp)  {
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
void _I8080_DAD(uint8_t rp) {
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
void _I8080_DAA() {
  uint8_t d8;
  d8 = 0;
  if  ( ((_rA & 0x0F) > 9) || (_getFlags_A() == 1))
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
  if  ( ( ((_rA & 0xF0) >> 4) > 9) || (_getFlags_C() == 1) || ( ( ((_rA & 0xF0) >> 4) >= 9) && ((_rA & 0x0F) > 9) )  )
  {
    if ( ( ((_rA & 0xF0) >> 4) > 9) || ( ( ((_rA & 0xF0) >> 4) >= 9) && ((_rA & 0x0F) > 9) ) ) {
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
void _I8080_ANA(uint8_t SSS) {
  if (SSS == _Reg_M)
  {
    _ACT = _rA;
    _AB = _rpHL;
    _RDMEM();
    _TMP = _DB;
  }
  else
  {
    _TMP = _Regs[SSS];
    _ACT = _rA;
  }
  //8080 - CY = 0, AC = bits 3 _rA OR d8
  _setFlags_C(0);
  if ( ((_ACT & B1000) | (_TMP & B1000)) != 0) {
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
void _I8080_ANI() {
  _PC++;
  _ACT = _rA;
  _AB = _PC;
  _RDMEM();
  _TMP = _DB;
  //8080 - CY = 0, AC = bits 3 _rA OR d8
  _setFlags_C(0);
  if ( ((_ACT & B1000) | (_TMP & B1000)) != 0) {
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
void _I8080_ORA(uint8_t SSS) {
  if (SSS == _Reg_M)
  {
    _ACT = _rA;
    _AB = _rpHL;
    _RDMEM();
    _TMP = _DB;
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
void _I8080_ORI() {
  _PC++;
  _ACT = _rA;
  _AB = _PC;
  _RDMEM();
  _TMP = _DB;
  _rA = _ACT | _TMP;
  _FLAGS(_rA);
  _setFlags_C(0);
  _setFlags_A(0);
  _PC++;
}

//XRA
void _I8080_XRA(uint8_t SSS) {
  if (SSS == _Reg_M)
  {
    _ACT = _rA;
    _AB = _rpHL;
    _RDMEM();
    _TMP = _DB;
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
void _I8080_XRI() {
  _PC++;
  _ACT = _rA;
  _AB = _PC;
  _RDMEM();
  _TMP = _DB;
  _rA = _ACT ^ _TMP;
  _FLAGS(_rA);
  _setFlags_C(0);
  _setFlags_A(0);
  _PC++;
}

//CMP
void _I8080_CMP(uint8_t SSS) {
  if (SSS != _Reg_M) {
    _ACT = _rA;
    _TMP = _Regs[SSS];
  }
  else {
    _ACT = _rA;
    _AB = _rpHL;
    _RDMEM();
    _TMP = _DB;
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
void _I8080_CPI() {
  _ACT = _rA;
  _PC++;
  _AB = _PC;
  _RDMEM();
  _TMP = _DB;
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
void _I8080_RRC() {
  _ALU = _rA;
  _setFlags_C(bitRead(_ALU, 0));
  _rA = _ALU >> 1;
  bitWrite(_rA, 7, _getFlags_C());
  _PC++;
}

//RLC
void _I8080_RLC() {
  _ALU = _rA;
  _setFlags_C(bitRead(_ALU, 7));
  _rA = _ALU << 1;
  bitWrite(_rA, 0, _getFlags_C());
  _PC++;
}

//RAL
void _I8080_RAL() {
  _ALU = _rA;
  _rA = ((_ALU << 1) & B11111110) | _getFlags_C();
  _setFlags_C(bitRead(_ALU, 7));
  _PC++;
}

//RAR
void _I8080_RAR() {
  _ALU = _rA;
  _rA = ((_ALU >> 1) & B01111111) | (_getFlags_C() << 7);
  _setFlags_C(bitRead(_ALU, 0));
  _PC++;
}

//CMA
void _I8080_CMA() {
  _rA = ~_rA;
  _PC++;
}

//CMC
void _I8080_CMC() {
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
void _I8080_STC() {
  _setFlags_C(1);
  _PC++;
}

//JMP
void _I8080_JMP() {
  _PC++;
  _AB = _PC;
  _RDMEM();
  _PC++;
  _Z = _DB;
  _AB = _PC;
  _RDMEM();
  _PC++;
  _W = _DB;
  _PC = _rpWZ;
}

//Jccc
void _I8080_JCCC(uint8_t CCC)
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
  _AB = _PC;
  _RDMEM();
  _PC++;
  _Z = _DB;
  _AB = _PC;
  _RDMEM();
  _PC++;
  _W = _DB;
  if (COND) {
    _PC = _rpWZ;
  }
}

//CALL
void _I8080_CALL() {
  _SP--;
  _PC++;
  _AB = _PC;
  _RDMEM();
  _PC++;
  _Z = _DB;
  _AB = _PC;
  _RDMEM();
  _PC++;
  _W = _DB;
  _AB = _SP;
  _DB = highByte(_PC);
  _WRMEM();
  _SP--;
  _AB = _SP;
  _DB = lowByte(_PC);
  _WRMEM();
  _PC = _rpWZ;
}

//Cccc
void _I8080_CCCC(uint8_t CCC) {
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
  _AB = _PC;
  _RDMEM();
  _PC++;
  _Z = _DB;
  _AB = _PC;
  _RDMEM();
  _PC++;
  _W = _DB;
  if (COND) {
    _AB = _SP;
    _DB = highByte(_PC);
    _WRMEM();
    _SP--;
    _AB = _SP;
    _DB = lowByte(_PC);
    _WRMEM();
    _PC = _rpWZ;
  }
}

//RET
void _I8080_RET()
{
  _AB = _SP;
  _RDMEM();
  _SP++;
  _Z = _DB;
  _AB = _SP;
  _RDMEM();
  _SP++;
  _W = _DB;
  _PC = _rpWZ;
}

//Rccc
void _I8080_RCCC(uint8_t CCC)
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
    _AB = _SP;
    _RDMEM();
    _SP++;
    _Z = _DB;
    _AB = _SP;
    _RDMEM();
    _SP++;
    _W = _DB;
    _PC = _rpWZ;
  }
  else {
    _PC++;
  }
}

//RST
void _I8080_RST(uint8_t n) {
  _PC++;
  pc2sp();
  _PC = n * 8;
}


//PCHL
void _I8080_PCHL() {
  _PC = _rpHL;
  _AB = _PC;
}

//XTHL
void _I8080_XTHL() {
  _AB = _SP;
  _RDMEM();
  _SP++;
  _Z = _DB;
  _AB = _SP;
  _RDMEM();
  _W = _DB;
  _AB = _SP;
  _DB = _rH;
  _WRMEM();
  _SP--;
  _AB = _SP;
  _DB = _rL;
  _WRMEM();
  _rL = _Z;
  _rH = _W;
  _PC++;
}

//SPHL
void _I8080_SPHL()
{
  _SP = _rpHL;
  _PC++;
}

//PUSH
void _I8080_PUSH(uint8_t rp) {
  _SP--;
  _AB = _SP;
  _SP--;
  switch (rp)  {
    case _RP_BC:
      _DB = _rB;
      break;
    case _RP_DE:
      _DB = _rD;
      break;
    case _RP_AF:
      _DB = _rA;
      break;
    case _RP_HL:
      _DB = _rH;
      break;
  }
  _WRMEM();
  _AB = _SP;
  switch (rp)  {
    case _RP_BC:
      _DB = _rC;
      break;
    case _RP_DE:
      _DB = _rE;
      break;
    case _RP_AF:
      _DB = _rF;
      break;
    case _RP_HL:
      _DB = _rL;
      break;
  }
  _WRMEM();
  _PC++;
}

//POP
void _I8080_POP(uint8_t rp) {
  _AB = _SP;
  _RDMEM();
  _SP++;
  switch (rp)  {
    case _RP_BC:
      _rC = _DB;
      break;
    case _RP_DE:
      _rE = _DB;
      break;
    case _RP_AF:
      _rF = _DB;
      break;
    case _RP_HL:
      _rL = _DB;
      break;
  }
  _AB = _SP;
  _RDMEM();
  _SP++;
  switch (rp)  {
    case _RP_BC:
      _rB = _DB;
      break;
    case _RP_DE:
      _rD = _DB;
      break;
    case _RP_AF:
      _rA = _DB;
      break;
    case _RP_HL:
      _rH = _DB;
      break;
  }
  _PC++;
}

//IN
void _I8080_IN() {
  _PC++;
  _AB = _PC;
  _RDMEM();
  _Z = _DB;
  _W = _DB;
  _AB = _rpWZ;
  _INPORT();
  _rA = _DB;
  _PC++;
}

//OUT
void _I8080_OUT() {
  _PC++;
  _AB = _PC;
  _RDMEM();
  _Z = _DB;
  _W = _DB;
  _AB = _rpWZ;
  _DB = _rA;
  _OUTPORT();
  _PC++;
}

//EI
void _I8080_EI() {
  INTE = true;
  _PC++;
}

//DI
void _I8080_DI() {
  INTE = false;
  _PC++;
}


