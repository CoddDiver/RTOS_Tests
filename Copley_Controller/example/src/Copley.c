/*
 * Copley.c
 *
 *  Created on: 21 Jul 2020
 *      Author: Andrew
 */
#include "Copley.h"
#include "board.h"
#include "UART_Manager.h"

#include "lwip/timers.h"

#include <stdio.h>
#include <string.h>

int poll = 0;
char Veldem[40];
int temp_dir = 0;
const char space2[2] = " ";
char *position;
char Response[32];

 int readCopleyResponse(char *ToRead) {
	char copResponse = UNKNOWN;
	if (strstr(ToRead, "v ") != NULL) {
		position = strtok(ToRead, "v ");
		Copley.Position = atoi(position);
		copResponse = POSITION;
	} else if (strstr(ToRead, "ok") != NULL) {
		copResponse = COPLEY_OK;
	} else if (strstr(ToRead, "e 10") != NULL) {
		copResponse = COPLEY_ERROR;
		DEBUGOUT("*****ERROR 10: DATA OUT OF RANGE****\r\n");
	} else if (strstr(ToRead, "e 33") != NULL) {
		copResponse = COPLEY_ERROR;
		DEBUGOUT("*****ERROR 33: PARSING ERROR****\r\n");
	} else if (strstr(ToRead, "e ") != NULL) {
		copResponse = COPLEY_ERROR;
		DEBUGOUT("ERROR = %s", ToRead);
		DEBUGOUT("***** GENERAL ERROR ****\r\n");
	}
	return copResponse;
}

void Copley_Disable(void){
	Send_UART("s r0x24 0\r");
	Copley_Read(COPLEY_OK);
}

void Copley_Enable(void){
	Send_UART("s r0x32 0\r"); // Set the pos register to correct value
	Copley_Read(COPLEY_OK);
	Send_UART("s r0x24 21\r");
	Copley_Read(COPLEY_OK);

}

void Copley_Configure() {
	Send_UART("g r0x32\r");

	Copley_Wait(20);

	DEBUGOUT("Reset\r\n");

	Send_UART("r\r");

	Copley_Wait(200);

	DEBUGOUT("Changing Baud Rate\r\n");
	Send_UART("s r0x90 115200\r");

	Copley_Wait(200);

	deinnit_UART_SELECTION();
	Setup_UART_SELECTION(115200);

	Copley_Wait(200);

	DEBUGOUT("Done Baud Rate\r\n");
	Send_UART("s r0x24 21\r"); //set desired sate to trajectory generator drives position loop
								// Also enables the drive
	Copley_Read(COPLEY_OK);

	Send_UART("s r0xc8 2\r"); //set trajectory profile mode to 2 = profile velocity mode
	Copley_Read(COPLEY_OK);

	Copley.Dir_Setting = -1;
	Send_UART("s r0xca 1\r"); // 1 count = jog in pos dir, -1 = jog in neg direction
	Copley_Read(COPLEY_OK);

	Copley_Wait(2);
	Send_UART("s r0xcc 1638400\r"); // set max accel to x in units of 10 counts/sec^2
	Copley_Read(COPLEY_OK);

	Send_UART("s r0xcd 1638400\r"); // set max decel to x in units of 10 counts/sec^2
	Copley_Read(COPLEY_OK);

	Send_UART("s r0xcb 0\r");  // Set velocity to x in units of 0.1 counts/sec
	Copley_Read(COPLEY_OK);

	Send_UART("t 1\r");  // trajectory update command
	Copley_Read(COPLEY_OK);

}

void Copley_Read(int _why){
	// _why is the response expected of the command you just sent
	// This needs a wrap around to break from no response
	Copley.Response = 0;
	while (Copley.Response != _why) {
		Poll_UART_For_String(Response);
		Copley.Response = readCopleyResponse(Response);
	}
}

void Copley_Send_Demand(int _speed) {
// _speed to be in units of motor encoder counts/s
	if (Copley.Read_Status >= 1) {

		if (_speed >= 0) {
			temp_dir = 1;
		} else {
			_speed = _speed * -1;  // can only send a positive demand
			temp_dir = -1;
		}
		if (temp_dir != Copley.Dir_Setting) {
			Copley.Dir_Setting = temp_dir;
			Set_Direction(Copley.Dir_Setting);
		} else {
			sprintf(Veldem, "s r0xcb %d\r", (_speed * 10)); // x10 because Copley needs units of 0.1 counts/s.

			Send_UART(Veldem);
			Copley_Read(COPLEY_OK);

			Send_UART("t 1\r");
			Copley_Read(COPLEY_OK);
		}

	}
}

void Set_Direction(int _dir) {
	if (_dir >= 0) {
		Send_UART("s r0xca 1\r"); // 1 count = jog in pos dir, -1 = jog in neg direction
		//DEBUGOUT("Change +\n\r");
	} else {
		Send_UART("s r0xca -1\r"); // 1 count = jog in pos dir, -1 = jog in neg direction
		//DEBUGOUT("Change - \n\r");
	}
	Copley_Read(COPLEY_OK);
}

void Copley_Get_Pos(void){
	if (Copley.Read_Status >= 1) {
		Send_UART("g r0x32\r");  // Read position register (in encoder counts)
		Copley_Read(POSITION);
	}
}

void Copley_Wait(int ms) {
	vTaskDelay(configTICK_RATE_HZ * ms / 100);
}
