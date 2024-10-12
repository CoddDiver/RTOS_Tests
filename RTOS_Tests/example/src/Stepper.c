/*
 * Stepper.c
 *
 *  Created on: 28 Sept 2020
 *      Author: Andrew
 */
#include "Stepper.h"
#include "board.h"

#include "lwip/timers.h"

#include <stdio.h>
#include <string.h>

int Steps = 0;
int temp_dir = 0;

void Stepper_Move(int _speed) {
// _speed to be in units of motor encoder counts/s
	if (Stepper.Case_Status == STEP_ENABLED) {

		if (_speed >= 0) {
			temp_dir = 1;
		} else {
			_speed = _speed * -1;  // can only send a positive demand
			temp_dir = -1;
		}
		if (temp_dir != Stepper.Dir_Setting) {
			Stepper.Dir_Setting = temp_dir;
			//Set_Direction(Stepper.Dir_Setting);
		} else {

		}

	}
}

void Stepper_Wait(int ms) {
	vTaskDelay(configTICK_RATE_HZ * ms / 100);
}

void Stepper_Disable() {
	COIL1(0);
	COIL2(0);
	COIL3(0);
	COIL4(0);
}

void Stepper_Step(int _dir) {

	if (_dir > 0) {
		Steps++;
		if (Steps > 7) {
			Steps = 0;
		}
	} else {
		Steps--;
		if (Steps < 0) {
			Steps = 7;
		}
	}
    Stepper.Steps_Progress++;

	switch (Steps) {
	case 0:
		COIL1(0);
		COIL2(0);
		COIL3(0);
		COIL4(1);
		break;
	case 1:
		COIL1(0);
		COIL2(0);
		COIL3(1);
		COIL4(1);
		break;
	case 2:
		COIL1(0);
		COIL2(0);
		COIL3(1);
		COIL4(0);
		break;
	case 3:
		COIL1(0);
		COIL2(1);
		COIL3(1);
		COIL4(0);

		break;
	case 4:
		COIL1(0);
		COIL2(1);
		COIL3(0);
		COIL4(0);
		break;
	case 5:
		COIL1(1);
		COIL2(1);
		COIL3(0);
		COIL4(0);
		break;
	case 6:
		COIL1(1);
		COIL2(0);
		COIL3(0);
		COIL4(0);
		break;
	case 7:
		COIL1(1);
		COIL2(0);
		COIL3(0);
		COIL4(1);
		break;
	default:
		COIL1(0);
		COIL2(0);
		COIL3(0);
		COIL4(0);
		break;
	}
}

void COIL1(int _set) {
	GPIO_Set(27, _set);
}
void COIL2(int _set) {
	GPIO_Set(28, _set);
}
void COIL3(int _set) {
	GPIO_Set(29, _set);
}
void COIL4(int _set) {
	GPIO_Set(30, _set);
}
