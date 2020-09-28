/*
 * Stepper.c
 *
 *  Created on: 28 Sept 2020
 *      Author: Andrew
 */
#include "Stepper.h"
#include "board.h"
#include "UART_Manager.h"

#include "lwip/timers.h"

#include <stdio.h>
#include <string.h>

int poll = 0;
char Veldem[40];
char Posdem[40];
int temp_dir = 0;
const char space2[2] = " ";
char *position;
char Response[32];


void Stepper_Send_Demand(int _speed) {
// _speed to be in units of motor encoder counts/s
	if (Stepper.Case_Status == COP_ENABLED) {

		if (_speed >= 0) {
			temp_dir = 1;
		} else {
			_speed = _speed * -1;  // can only send a positive demand
			temp_dir = -1;
		}
		if (temp_dir != Stepper.Dir_Setting) {
			Stepper.Dir_Setting = temp_dir;
			Set_Direction(Stepper.Dir_Setting);
		} else {

		}

	}
}

void Set_Direction(int _dir) {
	if (_dir >= 0) {

	} else {

	}

}

void Stepper_Get_Pos(void){



}
void Stepper_Set_Pos(float _pos){


}
void Stepper_Wait(int ms) {
	vTaskDelay(configTICK_RATE_HZ * ms / 100);
}
