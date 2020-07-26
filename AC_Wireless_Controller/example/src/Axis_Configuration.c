/*
 * Axis_Configuration.cpp
 *
 *  Created on: 31 Jan 2018
 *      Author: codda
 */

#include "Axis_Configuration.h"
#include <stdio.h>

// NOTE!!!! Must define (or not) QEI in header file depending on if the motor encoder is iC-MU or QEI.

void DriveAxisParameters(struct MyAxis *Axis, int mytype) {

	//if (mytype == COPLEY) {
		Axis->Sys_Counts = 4194304;
		Axis->Motor_Counts = 16000;
		Axis->Fade_Min_Speed = 0;  // Deg/s
		Axis->Max_Speed = 60;  // Deg/s
		Axis->Fade_Accel_Time = 6;  // s
	    Axis->Cut_Accel_Time = 6;   // s
		Axis->Joy_Decel_Time = 650; //ms
		Axis->Joy_SF = 1;  // Scales analogue input into counts/ms
		Axis->Ratio = 1;  // -152 on FHR35, 172 on FH145
		Axis->System_position_offset = 1;  // Deg, not yet implemented
		Axis->Direction = 1; // -1 or 1, inverts the direction of the motor demand   //  NONOTEC MOTOR = 1, HENGDRIVE = -1.
		Axis->Kp_Vel = 0;
		Axis->K_Assist = 0;
		Axis->Drag = 0;
		Axis->Kp_MotPos = 0;
		Axis->Kp_Sys = 0;
		Axis->Ki_Sys = 0;
		Axis->K_Diff = 0;
		Axis->Kff = 1;
		Axis->Kff_Sys = 1;  //0.8
		Axis->Sys_follow_limit = 5;  // Degrees

		Axis->flip_sin = false;  // Hengdrive Motor = 0, Nanotec DF45 = 1.


//	}
    Axis->Fade_Run_Time = 0;
	Axis->System_position = 0;
	Axis->Max_Limit = 357;
	Axis->Min_Limit = 3;
	Axis->Joy_Demand = 0;
	Axis->Feed_Forward = 0;
	Axis->Enable = false;
	Axis->Jog = 1; // -1.45; //counts / ms = 1 deg/s
	Axis->sysjogdir = 1;
	//Axis->scoping = 0;
	Axis->Command = 0;
	//Axis->moveme = 0;
	Axis->ramp = 0.05;
	Axis->Run_Count = 0;
	Axis->Motor_RPM = 0;
	Axis->Motor_Enc_Vel = 0;
	Axis->Fade_Time = 100;
	Axis->Fade_Target = 180;
	Axis->Soak_Target1 = 180;
	Axis->Soak_Target2 = 190;
	Axis->Fade_Speed = 2;  // Fade demand Deg/s
	Axis->Motor_position = 0;
	Axis->dojog = 0;
	Axis->sysdojog = 0;
	Axis->M7_Pos_Target = 0;

}

