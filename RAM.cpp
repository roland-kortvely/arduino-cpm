/**
 * Copyright (c) 2018 Roland Körtvely <roland.kortvely@gmail.com>
 */


#include "RAM.h"
#include "CONFIG.h"
#include "GPU.h"
#include "MEM.h"
#include "SD.h"

void RAM::init()
{

	GPU::block("RAM::init");

	if (SD::eraseSD(SD_MEM_OFFSET, 65536UL / CACHE_LINE_SIZE * MMU_BANKS_NUM) == false) {
		GPU::error();
	}

	//RAM::ERASE();

	GPU::ok();

	//RAM::MEM_TEST();
}

void RAM::ERASE() {
	uint8_t LRC;
	uint32_t blk;
	uint32_t blk_end;
	uint8_t res;

	blk = SD_MEM_OFFSET;
	blk_end = blk + (65536UL / CACHE_LINE_SIZE) * MMU_BANKS_NUM;

	do {
		LRC = 0;									//LRC reset
		for (uint32_t j = 0; j < CACHE_LINE_SIZE; j++) {
			SD::_buffer[j] = 0;
			LRC = 0 ^ LRC;							//LRC calculation
		}
		SD::_buffer[CACHE_LINE_SIZE] = LRC;			//LRC add
		res = SD::writeSD(blk);
		blk++;
	} while (blk < blk_end);
}

void RAM::MEM_TEST() {

	GPU::lnblockln("RAM TESTING");

	uint8_t CHECKED_BANKS;
	switch (MEM::RAM_TEST_MODE) {
	case 0:
		CHECKED_BANKS = 1;
		break;
	case 1:
		CHECKED_BANKS = MMU_BANKS_NUM;
		break;
	}

	for (uint8_t bank = 0; bank < CHECKED_BANKS; bank++) {

		GPU::print(F("BANK "));
		GPU::print(String(bank, HEX));

		for (uint8_t block = 0; block < MMU_BLOCKS_NUM; block++) {
			MEM::bank_set(block, bank);
		}

		CONFIG::RAM_AVAIL = MEM::MEM_TEST(true);

		GPU::print(String(CONFIG::RAM_AVAIL / 1024, DEC));
		GPU::println("K");

		if (CONFIG::RAM_AVAIL != 0x10000) {
			GPU::error();
		}
	}

	//ACTIVATE BANK 0
	for (uint8_t block = 0; block < MMU_BLOCKS_NUM; block++) {
		MEM::bank_set(block, 0);
	}

	GPU::print(String(CONFIG::RAM_AVAIL, DEC));
	GPU::print(" X ");
	GPU::print(String(MMU_BANKS_NUM, DEC));
	GPU::blockln(F(" BYTE(S) OF RAM ARE AVAILABLE"));

	GPU::block("RAM TESTING");
	GPU::ok();
}

