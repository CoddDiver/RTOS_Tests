/*
 * Stepper.h
 *
 *  Created on: 28 Sept 2020
 *      Author: Andrew
 */

#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

#define COPLEY_UNKNOWN 0
#define COPLEY_OK 1
#define COPLEY_ERROR 2
#define COPLEY_POSITION 3

#define COP_DISABLED 7
#define COP_TRY_DISABLE 8
#define COP_TRY_ENABLE 9
#define COP_ENABLED 10
#define COP_STARTING 11
#define COP_FAULT 12


struct MyStepper {
	int Case_Status;
	int Dir_Setting;
	int Response;
	int Position;  // Encoder counts
	int VelDemand; // Velocity in 1 counts/s
} Stepper;

void Stepper_Step();
void Stepper_Move(int _speed);
void Set_Direction(int _dir);
void Stepper_Get_Pos(void);
void Stepper_Set_Pos(float _pos);
void Stepper_Wait(int ms);
void COIL1(int _set);
void COIL2(int _set);
void COIL3(int _set);
void COIL4(int _set);
#endif /* INC_STEPPER_H_ */
