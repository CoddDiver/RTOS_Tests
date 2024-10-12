/*
 * Stepper.h
 *
 *  Created on: 28 Sept 2020
 *      Author: Andrew
 */

#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

#define STEPPER_UNKNOWN 0
#define STEPPER_OK 1
#define STEPPER_ERROR 2
#define STEPPER_POSITION 3

#define STEP_DISABLED 7
#define STEP_TRY_DISABLE 8
#define STEP_TRY_ENABLE 9
#define STEP_ENABLED 10
#define STEP_STARTING 11
#define STEP_FAULT 12


struct MyStepper {
	int Case_Status;
	int Dir_Setting;
	int Response;
	int Position;  // Steps
	int VelDemand; // Steps/time unit
	int Steps_Demand;
	int Steps_Progress;
} Stepper;

void Stepper_Disable();
void Stepper_Step(int _dir);
void Stepper_Move(int _speed);
void Stepper_Wait(int ms);
void COIL1(int _set);
void COIL2(int _set);
void COIL3(int _set);
void COIL4(int _set);
#endif /* INC_STEPPER_H_ */
