/*
 * myprint.c
 *
 *  Created on: 19 Jun 2020
 *      Author: Andrew
 */
#include "board.h"
#include <string.h>
#include <stdio.h>

char printme[100];
char _res[100];
char zero[64];
char zero_zero[64];
char minus[64];
char tmp[64];
char whole[64];
char decimal[10];
char decimalvals[64];
bool sign = false;

void ftoa(float n, char* res, int afterpoint)
{
     strcpy(minus, "-");
     tmp[0] = '\0';
	 whole[0] = '\0';
	 decimal[0] = '\0';
	 decimalvals[0] = '\0';

	strcpy(zero, "0");
	strcpy(zero_zero, "00");

	if (n >= 0){
		sign = false;
	} else {
		sign = true;
		n = n * -1;
	}
    // Extract integer part
    int ipart = (int)n;

    sprintf(whole, "%d", ipart); // add the integer value to whole string

    // Extract floating par
    float fpart = n - (float)ipart;

    int fpartn = ((int)(fpart * 1000));

    sprintf(decimalvals, "%d", fpartn); // add the fraction to the decimal string

    if (fpart < 0.01){
    	strcat(zero_zero, decimalvals);
    	 strncpy(decimal, zero_zero, 3);
    } else if (fpart < 0.1){
    	strcat(zero, decimalvals);
    	strncpy(decimal, zero, 3);
    } else {
    	strncpy(decimal, decimalvals, 3);
    }


    // check for display option after point
    if (afterpoint != 0) {
    	strcat(whole, ".");  //add the decimal
        // kill the decimal beyond the max points required
    	decimal[afterpoint] = '\0';
    	strcpy(tmp, whole);
    	strcat(tmp, decimal);
    } else {
    	strcpy(tmp, whole);
    }
    if (sign){
    	strcpy(res, minus);
    	strcat(res, tmp);
    } else {
    	strcpy(res, tmp);
    }


}

void MYDEBUG(char _char[], float _double, int _integer, int _type)
{
    _res[0]= '\0';
	switch(_type)
	{
	case 0:
		strcat(printme, _char);
		break;
	case 1:
		ftoa(_double, _res, 3);
		strcat(printme, _res);
		break;
	case 2:
		sprintf(_res, "%d", _integer);
		strcat(printme, _res);
		break;
	}
	DEBUGOUT("%s", printme);
	printme[0] = '\0';

}
