/*
 * EEPROM_Manager.h
 *
 *  Created on: 29 Jun 2020
 *      Author: Andrew Codd
 *  This sets up a page of memory for float values with addressing for the application to use.
 */

#ifndef SRC_EEPROM_MANAGER_H_
#define SRC_EEPROM_MANAGER_H_

#define PAGE 0 // Use Page 0 of memory, which has 64 bytes available.

#define FLOAT_0_SAVE(togo) 	SaveFloat(PAGE, 0, togo)
#define FLOAT_1_SAVE(togo) 	SaveFloat(PAGE , 4, togo)
#define FLOAT_2_SAVE(togo) 	SaveFloat(PAGE, 8, togo)
#define FLOAT_3_SAVE(togo) 	SaveFloat(PAGE, 12, togo)
#define FLOAT_4_SAVE(togo) 	SaveFloat(PAGE, 16, togo)
#define FLOAT_5_SAVE(togo) 	SaveFloat(PAGE, 20, togo)
#define FLOAT_6_SAVE(togo) 	SaveFloat(PAGE, 24, togo)
#define FLOAT_7_SAVE(togo) 	SaveFloat(PAGE, 28, togo)
#define FLOAT_8_SAVE(togo) 	SaveFloat(PAGE, 32, togo)
#define FLOAT_9_SAVE(togo) 	SaveFloat(PAGE, 36, togo)
#define FLOAT_10_SAVE(togo) SaveFloat(PAGE, 40, togo)
#define FLOAT_11_SAVE(togo) SaveFloat(PAGE, 44, togo)
#define FLOAT_12_SAVE(togo) SaveFloat(PAGE, 48, togo)
#define FLOAT_13_SAVE(togo) SaveFloat(PAGE, 52, togo)
#define FLOAT_14_SAVE(togo) SaveFloat(PAGE, 56, togo)
#define FLOAT_15_SAVE(togo) SaveFloat(PAGE, 60, togo)

#define FLOAT_0_READ 	ReadFloat(PAGE, 0)
#define FLOAT_1_READ 	ReadFloat(PAGE, 4)
#define FLOAT_2_READ 	ReadFloat(PAGE, 8)
#define FLOAT_3_READ 	ReadFloat(PAGE, 12)
#define FLOAT_4_READ	ReadFloat(PAGE, 16)
#define FLOAT_5_READ 	ReadFloat(PAGE, 20)
#define FLOAT_6_READ 	ReadFloat(PAGE, 24)
#define FLOAT_7_READ 	ReadFloat(PAGE, 28)
#define FLOAT_8_READ 	ReadFloat(PAGE, 32)
#define FLOAT_9_READ 	ReadFloat(PAGE, 36)
#define FLOAT_10_READ 	ReadFloat(PAGE, 40)
#define FLOAT_11_READ 	ReadFloat(PAGE, 44)
#define FLOAT_12_READ 	ReadFloat(PAGE, 48)
#define FLOAT_13_READ 	ReadFloat(PAGE, 52)
#define FLOAT_14_READ 	ReadFloat(PAGE, 56)
#define FLOAT_15_READ 	ReadFloat(PAGE, 60)


void SaveFloat(int page, int addr, float _val);
float ReadFloat(int page, int addr);

#endif /* SRC_EEPROM_MANAGER_H_ */
