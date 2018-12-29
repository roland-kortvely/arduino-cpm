/*  
    CREDITS
    CP/M 2.2 sources - http://www.cpm.z80.de/source.html
    CP/M 2.2 binaries - http://www.retroarchive.org/cpm/
    CP/M 2.2 & DRI binaries - http://www.gaby.de/cpm/binary.html
    binary file to a c/c++ array converter - http://tools.garry.tv/bin2c/
    Pretty 8080 Assembler - http://asdasd.rpg.fi/~svo/i8080/
    Cristian Maglie <c.maglie@bug.st> - SPI Master library for arduino
    William Greiman - Arduino Sd2Card Library
    Christian Weichel <info@32leaves.net> - PS2Keyboard library
*/

#include <avr/pgmspace.h>
#include "Sd2Card.h"
#include "PS2Keyboard.h"
#include "EEPROM.h"
//#include "Wire.h"

//version
const char VER_MAJOR = '0';
const char VER_MINOR = '1';

#include "ESC.h"

#include "SYS.h"

#include "CPM_def.h"

#include "SD.h"

#include "FRAM.h"

#include "MEM.h"

#include "FDD.h"

#include "CONIO.h"

#include "IO.h"

#include "i8080_exec.h"

//-----------------------------------------------------
//EEPRPOM
/*
EEPROM cells
               0xFE - 0x55
               0xFF - 0xAA
               0x00 - drive A
               0x01 - drive B
               0x02 - drive C
               0x03 - drive D
               0x04 - sense sw
*/
//EEPROM init
const int EEPROM_SIZE = 256;
const int EEPROM_DRIVES = 0x00;
const int EEPROM_SENSE_SW = EEPROM_DRIVES+FDD_NUM;
int EEPROM_idx;
void EEPROM_init() {
  //EEPROM clearing
       for (EEPROM_idx = 0 ; EEPROM_idx < EEPROM_SIZE ; EEPROM_idx++) {
          EEPROM.write(EEPROM_idx, 0);
       }
       //settings init
       for (EEPROM_idx = EEPROM_DRIVES ; EEPROM_idx < (EEPROM_DRIVES+FDD_NUM) ; EEPROM_idx++) {
        EEPROM.write(EEPROM_idx, uint8_t(EEPROM_idx-EEPROM_DRIVES));
       }
       //sense switches 0ff
       EEPROM.write(EEPROM_SENSE_SW, 0x00);
       //write signature to EEPROM
       //0xFE - 0x55
       EEPROM.write(0xFE, 0x55);
       //0xFF - 0xAA
       EEPROM.write(0xFF, 0xAA);
}

#include "BIOS.h"

#include "i8080_fns.h"

void call(word addr)
{
  byte cmd;
  bool exe_flag;
  exitFlag = false;
  _PC = addr;
  do
  {
    _AB = _PC;
    if (_AB ==  breakpoint) {
      DEBUG = true;
    }
    if (exitFlag) { break; } //go to monitor
    #include "BIOS_int.h"
    _RDMEM();//(AB) -> INSTR  instruction fetch
    _IR = _DB;
    #include "debug.h" 
    ((CmdFunction) pgm_read_word (&doCmdArray [_IR])) (); //decode
  } while (true);
  if (MEM_ERR) {
    MEM_ERR = false;
    clrscr();
    Serial.println("");
    Serial.println(F("MEMORY ERROR!"));
  }
}

//reset function
void(* sys_reset) (void) = 0;

void setup() {
  uint32_t i;
  uint16_t j;
  uint8_t k;
  uint32_t _cardsize;
  uint8_t res;
  bool RAMTestPass = true;
  uint8_t LRC;
  uint32_t blk;
  uint32_t blk_end;
  uint32_t start_time;
  uint8_t bank;
  uint8_t block;
  uint8_t CHECKED_BANKS;
  // start serial port at 9600 bps
  Serial.begin(9600);
  while (!Serial) {
    ; //wait for serial port to connect. Needed for native USB port only
  }
  delay(1000);
  clrscr();
  //pins setup
  pinMode(LED_pin, OUTPUT);
  digitalWrite(LED_pin, LOW);
  pinMode(IN_pin, INPUT);
  pinMode(OUT_pin, OUTPUT);
  digitalWrite(OUT_pin, LOW);
  //Timer1 setup
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 312;//50 Hz
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);//prescaler 1024
  TIMSK1 |= (1 << OCIE1A);//int enable
  //TIMSK1 = 0;//int disable
  sei();
  //logo
  color(2);
  Serial.println(F("***************************************"));
  Serial.println(F("*      (C) 2018 Roland Kortvely       *"));
  Serial.println(F("***************************************"));
  Serial.println("");
  
/*
asm (
  "1:         \n"
  "nop        \n" //repeating code goes here
  );
  Serial.print("T");
  delay(100);
asm (
  "jmp 1b        \n"
  );
*/  

  
  //EEPROM checking
  if ((EEPROM.read(0xFE) != 0x55) || (EEPROM.read(0xFF) != 0xAA)) {
     //EEPROM init
     EEPROM_init();
  }
  
  //disks mount
  for (k=0; k<FDD_NUM; k++) {
    SD_FDD_OFFSET[k] = SD_DISKS_OFFSET + EEPROM.read(k)*SD_DISK_SIZE; 
  }  
  
  //sense switch
  SENSE_SW = EEPROM.read(EEPROM_SENSE_SW);
  
  //flush serial buffer
  con_flush();
  
  //MMU init
  for (i = 0; i < MMU_BLOCKS_NUM; i++) {
    MMU_MAP[i] = 0;
  }
  MMU_BLOCK_SEL_REG = 0;
  
  //cache init
  for (i = 0; i < CACHE_LINES_NUM; i++) {
    cache_tag[i] = 0xFFFFFFFF;
  }
  for (i = 0; i < CACHE_LINES_NUM; i++) {
    cache_dirty[i] = true;
  }
  for (i = 0; i < CACHE_LINES_NUM; i++) {
    cache_start[i] = i * CACHE_LINE_SIZE;
  }
  
  //SD card init
  Serial.print(F("SD CARD INIT..."));
  do {
    card.init(SPI_FULL_SPEED, SS_SD_pin);
    _cardsize = card.cardSize();
    if (_cardsize != 0) {
      Serial.println(F("O.K."));
      Serial.print(F("CARD SIZE: "));
      Serial.print(_cardsize);
      Serial.println(F(" SECTORS"));
    }
    else {
      delay(250);
    }
  } while (_cardsize == 0);

  //SD RAM AREA CLEARING
  Serial.print(F("SD RAM AREA ERASING..."));
  
  if (eraseSD(SD_MEM_OFFSET,65536UL/CACHE_LINE_SIZE*MMU_BANKS_NUM) == false) {
    Serial.println(F("ERROR!"));
    while(1);
  }
  else
  {
    Serial.println(F("O.K."));
  }
 
  
  Serial.print(F("SD RAM AREA INIT"));
/*
  blk = SD_MEM_OFFSET;
  blk_end = blk + (65536UL/CACHE_LINE_SIZE) * MMU_BANKS_NUM;
  i = 0;
  do {
    LRC = 0;//LRC reset
    for(j=0;j<CACHE_LINE_SIZE;j++) {
      _buffer[j] = 0;
      LRC = 0 ^ LRC;//LRC calculation
    }
    _buffer[CACHE_LINE_SIZE] = LRC;//LRC add
    res = writeSD(blk);
    blk++;
    if ((i % ((65536UL/CACHE_LINE_SIZE))) == 0) {
      Serial.print(".");
    }
    i++;
  } while (blk < blk_end);
  Serial.println(F("O.K."));
  */
  /*
  Serial.println(F("SELECT BANK(S) FOR TEST: "));
  Serial.println(F("[0] - BANK 0, [1] - ALL BANKS"));
  */
  RAM_TEST_MODE = 0xFF;
  start_time = millis();
  do {
    if (con_ready()) {
      inChar = con_read();
      switch (inChar) {
        case '0': RAM_TEST_MODE = 0;
          break;
        case '1': RAM_TEST_MODE = 1;
          break;
      }
    }
  } while ((RAM_TEST_MODE == 0xFF) && ((millis() - start_time) < SET_PAUSE));
  if (RAM_TEST_MODE == 0xFF) {
    RAM_TEST_MODE = 0x0;//Bank 0 check default
  }
  
  //Serial.print(F("RAM TEST..."));
  switch (RAM_TEST_MODE) {
    case 0: //Serial.println(F("BANK 0"));  
            CHECKED_BANKS = 1;
            break;
    case 1: //Serial.println(F("ALL BANKS"));
            CHECKED_BANKS = MMU_BANKS_NUM;
            break;
  }
  

  //MEMORY SPEED TEST
  /*
  uint32_t xxlen;
  uint16_t xxxx;
  uint32_t xxcnt;
  uint8_t xx;
  xxcnt = 0;//counter
  xxxx = 0xABCD;//seed
  xxlen = 20000;//reads number
  Serial.println(F("MEMORY SPEED TEST"));
  Serial.println(F("RND: START"));
  do {
    xxxx ^= xxxx << 2;
    xxxx ^= xxxx >> 7;
    xxxx ^= xxxx << 7;
    _AB = xxxx;
    _RDMEM();
    xx = _DB;
    xxcnt++;
  } while (xxcnt < xxlen);
  Serial.println(F("RND: STOP"));
  Serial.println(xxlen);//reads number
  xxxx = 0;
  xxlen = 100000;
  Serial.println(F("SEQ: START"));
  do {
    _AB = xxxx;
    _RDMEM();
    xx = _DB;
    xxxx++;
    //Serial.println(rnd);
    xxcnt++;
  } while (xxcnt < xxlen);
  Serial.println(F("SEQ: STOP"));
  Serial.println(xxlen);//reads number
  */
  
  //RAM TEST
  
  for(bank=0;bank<CHECKED_BANKS;bank++) {
    Serial.print(F("BANK "));
    Serial.print(bank, HEX);
    //bank set for all blocks
    for (block=0;block<MMU_BLOCKS_NUM;block++) {
      bank_set(block,bank);
    }
    RAM_AVAIL = mem_test(false);
    Serial.print(RAM_AVAIL/1024, DEC);
    Serial.print("K");
    if (RAM_AVAIL != 0x10000) {
      Serial.println("");
      Serial.println("RAM CHECK ERROR!");
      delay(3000);
      sys_reset();
    }
    Serial.println("");
  }
  
  //BANK 0 ACTIVE
  for (block=0;block<MMU_BLOCKS_NUM;block++) {
      bank_set(block,0);
  }
  Serial.print(RAM_AVAIL, DEC);
  Serial.print(" X ");
  Serial.print(MMU_BANKS_NUM, DEC);
  Serial.println(F(" BYTE(S) OF RAM ARE AVAILABLE"));
 

 Serial.println(F("BOOT.. OK"));
 delay(1500);
  
  color(9);
  //stack init
  _SP = SP_INIT;
  DEBUG = true;//debug on
  delay(1000);
  clrscr();//clear screen
  xy(MON_Y, 0);//cursor positioning
  Serial.print('>');
}

#include "TMR.h"

void loop() {
  
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
      inChar = '\0';
      if (con_ready()) {
        inChar = con_read();
        //Serial.print(uint8_t(inChar),HEX);
        inChar = upCase(inChar);
        if (uint8_t(inChar) == BS_KEY) {
          //backspace
          if (mon_ptr > 0) {
            mon_ptr--;
            mon_buffer[mon_ptr] = '\0';
            Serial.write(inChar);
            Serial.write(" ");
            Serial.write(inChar);
          }
        }
        else {
          mon_buffer[mon_ptr] = inChar;
          mon_ptr++;
          Serial.write(inChar);
        }
      }
    } while ((inChar != '\r') && (inChar != '\n') && (mon_ptr < MON_BUFFER_SIZE));
    Serial.print('\n');
    mon_ptr = 0;
    #include "mon.h"//monitor command execution
    xy(MON_Y, 0);//cursor positioning
    Serial.print('>');
    clrend();//clear input line to end
  }
}
