/*  CPM4NANO - i8080 & CP/M emulator for Arduino Nano 3.0 
*   Copyright (C) 2017 - Alexey V. Voronin @ FoxyLab 
*   Email:    support@foxylab.com
*   Website:  https://acdc.foxylab.com
*/

   //dest = (cmd & ~__RP) >> 4;
   //reg = cmd & ~__SSS;
   //reg = (cmd & ~__DDD) >> 3;
   //rp = (cmd & ~__RP) >> 4;

    void do0x00 () { 
      _I8080_NOP();
      }
    void do0x01 () { 
      _I8080_LXI(_RP_BC);
      }
    void do0x02 () { 
      _I8080_STAX(_RP_BC);
      }
    void do0x03 () { 
      _I8080_INX(_Reg_B);
      }
    void do0x04 () { 
      _I8080_INR(_Reg_B);
      }
    void do0x05 () { 
      _I8080_DCR(_Reg_B);
      }
    void do0x06 () { 
      _I8080_MVI(_Reg_B);
      }
    void do0x07 () { 
      _I8080_RLC();
      }
    void do0x08 () { 
      _I8080_();//*NOP
      }
    void do0x09 () { 
      _I8080_DAD(_RP_BC);
      }
    void do0x0A () { 
      _I8080_LDAX(_RP_BC);
      }
    void do0x0B () { 
      _I8080_DCX(_Reg_B);
      }
    void do0x0C () { 
      _I8080_INR(_Reg_C);
      }
    void do0x0D () { 
      _I8080_DCR(_Reg_C);
      }
    void do0x0E () { 
      _I8080_MVI(_Reg_C);
      }
    void do0x0F () { 
      _I8080_RRC();
      }

    //10
    void do0x10 () { 
      _I8080_();//*NOP
      }
    void do0x11 () { 
      _I8080_LXI(_RP_DE);
      }
    void do0x12 () { 
      _I8080_STAX(_RP_DE);
      }
    void do0x13 () { 
      _I8080_INX(_RP_DE);
      }
    void do0x14 () { 
      _I8080_INR(_Reg_D);
      }
    void do0x15 () { 
      _I8080_DCR(_Reg_D);
      }
    void do0x16 () { 
      _I8080_MVI(_Reg_D);
      }
    void do0x17 () { 
      _I8080_RAL();
      }
    void do0x18 () { 
      _I8080_();//*NOP
      }
    void do0x19 () { 
      _I8080_DAD(_RP_DE);
      }
    void do0x1A () { 
      _I8080_LDAX(_RP_DE);
      }
    void do0x1B () { 
      _I8080_DCX(_RP_DE);
      }
    void do0x1C () { 
      _I8080_INR(_Reg_E);
      }
    void do0x1D () { 
      _I8080_DCR(_Reg_E);
      }
    void do0x1E () { 
      _I8080_MVI(_Reg_E);
      }
    void do0x1F () { 
      _I8080_RAR();
      }

    //20
    void do0x20 () { 
      _I8080_();//*NOP
      }
    void do0x21 () { 
      _I8080_LXI(_RP_HL);
      }
    void do0x22 () { 
      _I8080_SHLD();
      }
    void do0x23 () { 
      _I8080_INX(_RP_HL);
      }
    void do0x24 () { 
      _I8080_INR(_Reg_H);
      }
    void do0x25 () { 
      _I8080_DCR(_Reg_H);
      }
    void do0x26 () { 
      _I8080_MVI(_Reg_H);
      }
    void do0x27 () { 
      _I8080_DAA();
      }
    void do0x28 () { 
      _I8080_();//*NOP
      }
    void do0x29 () { 
      _I8080_DAD(_RP_HL);
      }
    void do0x2A () { 
      _I8080_LHLD();
      }
    void do0x2B () { 
      _I8080_DCX(_RP_HL);
      }
    void do0x2C () { 
      _I8080_INR(_Reg_L);
      }
    void do0x2D () { 
      _I8080_DCR(_Reg_L);
      }
    void do0x2E () { 
      _I8080_MVI(_Reg_L);
      }
    void do0x2F () { 
      _I8080_CMA();
      }


    //30
    void do0x30 () { 
      _I8080_();//*NOP
      }
    void do0x31 () { 
      _I8080_LXI(_RP_SP);
      }
    void do0x32 () { 
      _I8080_STA();
      }
    void do0x33 () { 
      _I8080_INX(_RP_SP);
      }
    void do0x34 () { 
      _I8080_INR(_Reg_M);
      }
    void do0x35 () { 
      _I8080_DCR(_Reg_M);
      }
    void do0x36 () { 
      _I8080_MVI(_Reg_M);
      }
    void do0x37 () { 
      _I8080_STC();
      }
    void do0x38 () { 
      _I8080_();//*NOP
      }
    void do0x39 () { 
      _I8080_DAD(_RP_SP);
      }
    void do0x3A () { 
      _I8080_LDA();
      }
    void do0x3B () { 
      _I8080_DCX(_RP_SP);
      }
    void do0x3C () { 
      _I8080_INR(_Reg_A);
      }
    void do0x3D () { 
      _I8080_DCR(_Reg_A);
      }
    void do0x3E () { 
      _I8080_MVI(_Reg_A);
      }
    void do0x3F () { 
      _I8080_CMC();
      }


    //40 MOV dest src
    void do0x40 () { 
      _I8080_MOV(_Reg_B, _Reg_B);
      }
    void do0x41 () { 
      _I8080_MOV(_Reg_B, _Reg_C);
      }
    void do0x42 () { 
      _I8080_MOV(_Reg_B, _Reg_D);
      }
    void do0x43 () { 
      _I8080_MOV(_Reg_B, _Reg_E);
      }
    void do0x44 () { 
      _I8080_MOV(_Reg_B, _Reg_H);
      }
    void do0x45 () { 
      _I8080_MOV(_Reg_B, _Reg_L);
      }
    void do0x46 () { 
      _I8080_MOV(_Reg_B, _Reg_M);
      }
    void do0x47 () { 
      _I8080_MOV(_Reg_B, _Reg_A);
      }
    void do0x48 () { 
      _I8080_MOV(_Reg_C, _Reg_B);
      }
    void do0x49 () { 
      _I8080_MOV(_Reg_C, _Reg_C);
      }
    void do0x4A () { 
      _I8080_MOV(_Reg_C, _Reg_D);
      }
    void do0x4B () { 
      _I8080_MOV(_Reg_C, _Reg_E);
      }
    void do0x4C () { 
      _I8080_MOV(_Reg_C, _Reg_H);
      }
    void do0x4D () { 
      _I8080_MOV(_Reg_C, _Reg_L);
      }
    void do0x4E () { 
      _I8080_MOV(_Reg_C, _Reg_M);
      }
    void do0x4F () { 
      _I8080_MOV(_Reg_C, _Reg_A);
      }

    //50
    void do0x50 () { 
      _I8080_MOV(_Reg_D, _Reg_B);
      }
    void do0x51 () { 
      _I8080_MOV(_Reg_D, _Reg_C);
      }
    void do0x52 () { 
      _I8080_MOV(_Reg_D, _Reg_D);
      }
    void do0x53 () { 
      _I8080_MOV(_Reg_D, _Reg_E);
      }
    void do0x54 () { 
      _I8080_MOV(_Reg_D, _Reg_H);
      }
    void do0x55 () { 
      _I8080_MOV(_Reg_D, _Reg_L);
      }
    void do0x56 () { 
      _I8080_MOV(_Reg_D, _Reg_M);
      }
    void do0x57 () { 
      _I8080_MOV(_Reg_D, _Reg_A);
      }
    void do0x58 () { 
      _I8080_MOV(_Reg_E, _Reg_B);
      }
    void do0x59 () { 
      _I8080_MOV(_Reg_E, _Reg_C);
      }
    void do0x5A () { 
      _I8080_MOV(_Reg_E, _Reg_D);
      }
    void do0x5B () { 
      _I8080_MOV(_Reg_E, _Reg_E);
      }
    void do0x5C () { 
      _I8080_MOV(_Reg_E, _Reg_H);
      }
    void do0x5D () { 
      _I8080_MOV(_Reg_E, _Reg_L);
      }
    void do0x5E () { 
      _I8080_MOV(_Reg_E, _Reg_M);
      }
    void do0x5F () { 
      _I8080_MOV(_Reg_E, _Reg_A);
      }

    //60
    void do0x60 () { 
      _I8080_MOV(_Reg_H, _Reg_B);
      }
    void do0x61 () { 
      _I8080_MOV(_Reg_H, _Reg_C);
      }
    void do0x62 () { 
      _I8080_MOV(_Reg_H, _Reg_D);
      }
    void do0x63 () { 
      _I8080_MOV(_Reg_H, _Reg_E);
      }
    void do0x64 () { 
      _I8080_MOV(_Reg_H, _Reg_H);
      }
    void do0x65 () { 
      _I8080_MOV(_Reg_H, _Reg_L);
      }
    void do0x66 () { 
      _I8080_MOV(_Reg_H, _Reg_M);
      }
    void do0x67 () { 
      _I8080_MOV(_Reg_H, _Reg_A);
      }
    void do0x68 () { 
      _I8080_MOV(_Reg_L, _Reg_B);
      }
    void do0x69 () { 
      _I8080_MOV(_Reg_L, _Reg_C);
      }
    void do0x6A () { 
      _I8080_MOV(_Reg_L, _Reg_D);
      }
    void do0x6B () { 
      _I8080_MOV(_Reg_L, _Reg_E);
      }
    void do0x6C () { 
      _I8080_MOV(_Reg_L, _Reg_H);
      }
    void do0x6D () { 
      _I8080_MOV(_Reg_L, _Reg_L);
      }
    void do0x6E () { 
      _I8080_MOV(_Reg_L, _Reg_M);
      }
    void do0x6F () { 
      _I8080_MOV(_Reg_L, _Reg_A);
      }

    //70
    void do0x70 () { 
      _I8080_MOV(_Reg_M, _Reg_B);
      }
    void do0x71 () { 
      _I8080_MOV(_Reg_M, _Reg_C);
      }
    void do0x72 () { 
      _I8080_MOV(_Reg_M, _Reg_D);
      }
    void do0x73 () { 
      _I8080_MOV(_Reg_M, _Reg_E);
      }
    void do0x74 () { 
      _I8080_MOV(_Reg_M, _Reg_H);
      }
    void do0x75 () { 
      _I8080_MOV(_Reg_M, _Reg_L);
      }
    void do0x76 () { 
      _I8080_HLT();
      }
    void do0x77 () { 
      _I8080_MOV(_Reg_M, _Reg_A);
      }
    void do0x78 () { 
      _I8080_MOV(_Reg_A, _Reg_B);
      }
    void do0x79 () { 
      _I8080_MOV(_Reg_A, _Reg_C);
      }
    void do0x7A () { 
      _I8080_MOV(_Reg_A, _Reg_D);
      }
    void do0x7B () { 
      _I8080_MOV(_Reg_A, _Reg_E);
      }
    void do0x7C () { 
      _I8080_MOV(_Reg_A, _Reg_H);
      }
    void do0x7D () { 
      _I8080_MOV(_Reg_A, _Reg_L);
      }
    void do0x7E () { 
      _I8080_MOV(_Reg_A, _Reg_M);
      }
    void do0x7F () { 
      _I8080_MOV(_Reg_A, _Reg_A);
      }


    //80
    void do0x80 () { 
      _I8080_ADD(_Reg_B);
      }
    void do0x81 () { 
      _I8080_ADD(_Reg_C);
      }
    void do0x82 () { 
      _I8080_ADD(_Reg_D);
      }
    void do0x83 () { 
      _I8080_ADD(_Reg_E);
      }
    void do0x84 () { 
      _I8080_ADD(_Reg_H);   
      }
    void do0x85 () { 
      _I8080_ADD(_Reg_L);
      }
    void do0x86 () { 
      _I8080_ADD(_Reg_M);
      }
    void do0x87 () { 
      _I8080_ADD(_Reg_A);
      }
    void do0x88 () { 
      _I8080_ADC(_Reg_B);
      }
    void do0x89 () { 
      _I8080_ADC(_Reg_C);
      }
    void do0x8A () { 
      _I8080_ADC(_Reg_D);
      }
    void do0x8B () { 
      _I8080_ADC(_Reg_E);
      }
    void do0x8C () { 
      _I8080_ADC(_Reg_H);
      }
    void do0x8D () { 
      _I8080_ADC(_Reg_L);
      }
    void do0x8E () { 
      _I8080_ADC(_Reg_M);
      }
    void do0x8F () { 
      _I8080_ADC(_Reg_A);
      }

    //90
    void do0x90 () { 
      _I8080_SUB(_Reg_B);
      }
    void do0x91 () { 
      _I8080_SUB(_Reg_C);
      }
    void do0x92 () { 
      _I8080_SUB(_Reg_D);
      }
    void do0x93 () { 
      _I8080_SUB(_Reg_E);
      }
    void do0x94 () { 
      _I8080_SUB(_Reg_H);
      }
    void do0x95 () { 
      _I8080_SUB(_Reg_L);
      }
    void do0x96 () { 
      _I8080_SUB(_Reg_M);
      }
    void do0x97 () { 
      _I8080_SUB(_Reg_A);
      }
    void do0x98 () { 
      _I8080_SBB(_Reg_B);
      }
    void do0x99 () { 
      _I8080_SBB(_Reg_C);
      }
    void do0x9A () { 
      _I8080_SBB(_Reg_D);
      }
    void do0x9B () { 
      _I8080_SBB(_Reg_E);
      }
    void do0x9C () { 
      _I8080_SBB(_Reg_H);
      }
    void do0x9D () { 
      _I8080_SBB(_Reg_L);
      }
    void do0x9E () { 
      _I8080_SBB(_Reg_M);
      }
    void do0x9F () { 
      _I8080_SBB(_Reg_A);
      }

    //A0
    void do0xA0 () { 
      _I8080_ANA(_Reg_B);
      }
    void do0xA1 () { 
      _I8080_ANA(_Reg_C);
      }
    void do0xA2 () { 
      _I8080_ANA(_Reg_D);
      }
    void do0xA3 () { 
      _I8080_ANA(_Reg_E);
      }
    void do0xA4 () { 
      _I8080_ANA(_Reg_H);
      }
    void do0xA5 () { 
      _I8080_ANA(_Reg_L);
      }
    void do0xA6 () { 
      _I8080_ANA(_Reg_M);
      }
    void do0xA7 () { 
      _I8080_ANA(_Reg_A);
      }
    void do0xA8 () { 
      _I8080_XRA(_Reg_B);
      }
    void do0xA9 () { 
      _I8080_XRA(_Reg_C);
      }
    void do0xAA () { 
      _I8080_XRA(_Reg_D);
      }
    void do0xAB () { 
      _I8080_XRA(_Reg_E);
      }
    void do0xAC () { 
      _I8080_XRA(_Reg_H);
      }
    void do0xAD () { 
      _I8080_XRA(_Reg_L);
      }
    void do0xAE () { 
      _I8080_XRA(_Reg_M);
      }
    void do0xAF () { 
      _I8080_XRA(_Reg_A);
      }

    //B0
    void do0xB0 () { 
      _I8080_ORA(_Reg_B);
      }
    void do0xB1 () { 
      _I8080_ORA(_Reg_C);
      }
    void do0xB2 () { 
      _I8080_ORA(_Reg_D);
      }
    void do0xB3 () { 
      _I8080_ORA(_Reg_E);
      }
    void do0xB4 () { 
      _I8080_ORA(_Reg_H);
      }
    void do0xB5 () { 
      _I8080_ORA(_Reg_L);
      }
    void do0xB6 () { 
      _I8080_ORA(_Reg_M);
      }
    void do0xB7 () { 
      _I8080_ORA(_Reg_A);
      }
    void do0xB8 () { 
      _I8080_CMP(_Reg_B);
      }
    void do0xB9 () { 
      _I8080_CMP(_Reg_C);
      }
    void do0xBA () { 
      _I8080_CMP(_Reg_D);
      }
    void do0xBB () { 
      _I8080_CMP(_Reg_E);
      }
    void do0xBC () { 
      _I8080_CMP(_Reg_H);
      }
    void do0xBD () { 
      _I8080_CMP(_Reg_L);
      }
    void do0xBE () { 
      _I8080_CMP(_Reg_M);
      }
    void do0xBF () { 
      _I8080_CMP(_Reg_A);
      }

    //C0
    void do0xC0 () { 
      _I8080_RCCC(B000);
      }
    void do0xC1 () { 
      _I8080_POP(_RP_BC);
      }
    void do0xC2 () { 
      _I8080_JCCC(B000);
      }
    void do0xC3 () { 
      _I8080_JMP();
      }
    void do0xC4 () { 
      _I8080_CCCC(B000);
      }
    void do0xC5 () { 
      _I8080_PUSH(_RP_BC);
      }
    void do0xC6 () { 
      _I8080_ADI();
      }
    void do0xC7 () { 
      _I8080_RST(0);
      }
    void do0xC8 () { 
      _I8080_RCCC(B001);
      }
    void do0xC9 () { 
      _I8080_RET();
      }
    void do0xCA () { 
      _I8080_JCCC(B001);
      }
    void do0xCB () { 
      _I8080_JMP();
      }
    void do0xCC () { 
      _I8080_CCCC(B001);
      }
    void do0xCD () { 
      _I8080_CALL();
      }
    void do0xCE () { 
      _I8080_ACI();
      }
    void do0xCF () { 
      _I8080_RST(1);
      }

    //D0
    void do0xD0 () { 
      _I8080_RCCC(B010);
      }
    void do0xD1 () { 
      _I8080_POP(_RP_DE);
      }
    void do0xD2 () { 
      _I8080_JCCC(B010);
      }
    void do0xD3 () { 
      _I8080_OUT();
      }
    void do0xD4 () { 
      _I8080_CCCC(B010);
      }
    void do0xD5 () { 
      _I8080_PUSH(_RP_DE);
      }
    void do0xD6 () { 
      _I8080_SUI();
      }
    void do0xD7 () { 
      _I8080_RST(2);
      }
    void do0xD8 () { 
      _I8080_RCCC(B011);
      }
    void do0xD9 () { 
      _I8080_RET();
      }
    void do0xDA () { 
      _I8080_JCCC(B011);
      }
    void do0xDB () { 
      _I8080_IN();
      }
    void do0xDC () { 
      _I8080_CCCC(B011);
      }
    void do0xDD () { 
      _I8080_CALL();
      }
    void do0xDE () { 
      _I8080_SBI();
      }
    void do0xDF () { 
      _I8080_RST(3);
      }

    //E0
    void do0xE0 () { 
      _I8080_RCCC(B100);
      }
    void do0xE1 () { 
      _I8080_POP(_RP_HL);
      }
    void do0xE2 () { 
      _I8080_JCCC(B100);
      }
    void do0xE3 () { 
      _I8080_XTHL();
      }
    void do0xE4 () { 
      _I8080_CCCC(B100);
      }
    void do0xE5 () { 
      _I8080_PUSH(_RP_HL);
      }
    void do0xE6 () { 
      _I8080_ANI();
      }
    void do0xE7 () { 
      _I8080_RST(4);
      }
    void do0xE8 () { 
      _I8080_RCCC(B101);
      }
    void do0xE9 () { 
      _I8080_PCHL();
      }
    void do0xEA () { 
      _I8080_JCCC(B101);
      }
    void do0xEB () { 
      _I8080_XCHG();
      }
    void do0xEC () { 
      _I8080_CCCC(B101);
      }
    void do0xED () { 
      _I8080_CALL();
      }
    void do0xEE () { 
      _I8080_XRI();
      }
    void do0xEF () { 
      _I8080_RST(5);
      }

    //F0
    void do0xF0 () { 
      _I8080_RCCC(B110);
      }
    void do0xF1 () { 
      _I8080_POP(_RP_AF);
      }
    void do0xF2 () { 
      _I8080_JCCC(B110);
      }
    void do0xF3 () { 
      _I8080_DI();
      }
    void do0xF4 () { 
      _I8080_CCCC(B110);
      }
    void do0xF5 () { 
      _I8080_PUSH(_RP_AF);
      }
    void do0xF6 () { 
      _I8080_ORI();
      }
    void do0xF7 () { 
      _I8080_RST(6);
      }
    void do0xF8 () { 
      _I8080_RCCC(B111);
      }
    void do0xF9 () { 
      _I8080_SPHL();
      }
    void do0xFA () { 
      _I8080_JCCC(B111);
      }
    void do0xFB () { 
      _I8080_EI();
      }
    void do0xFC () { 
      _I8080_CCCC(B111);
      }
    void do0xFD () { 
      _I8080_CALL();
      }
    void do0xFE () { 
      _I8080_CPI();
      }
    void do0xFF () { 
      _I8080_RST(7);
      }

 typedef void (*CmdFunction) ();
 
  // array of function pointers
  const CmdFunction doCmdArray [] PROGMEM =
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
