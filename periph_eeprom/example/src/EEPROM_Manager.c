/*
 * EEPROM_Manager.c
 *
 *  Created on: 29 Jun 2020
 *      Author: Andrew
 * *This sets up a page of memory for float values with addressing for the application to use.
 * The following details are needed, taken from eeprom_17xx_40xx.h, and apply to the NXP4088 chip
#define EEPROM_PAGE_SIZE                64		< EEPROM byes per page
#define EEPROM_PAGE_NUM                 63		<  EEPROM pages
 */

#include "EEPROM_Manager.h"
#include "board.h"
#include "string.h"



void SaveFloat(int page, int addr, float _val){
	Chip_EEPROM_Write(LPC_EEPROM, page, addr, &_val, EEPROM_RWSIZE_32BITS, (4));
}

float ReadFloat(int page, int addr){
	float _toread;
	Chip_EEPROM_Read(LPC_EEPROM, page, addr, &_toread, EEPROM_RWSIZE_32BITS, 4);
    return _toread;
}






