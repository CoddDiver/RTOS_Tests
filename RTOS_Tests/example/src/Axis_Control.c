/*
 * Axis_Control.cpp
 *
 *  Created on: 30 Jan 2018
 *      Author: codda
 */

#include "Profiler.h"
#include "Axis_Configuration.h"
#include "Axis_Control.h"
#include "MyAdmin.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

//extern struct MyAxis Drive;
extern void Main_Admin();



extern int run;

int vel_timer = 0;

bool running = false;
bool ignore = false;

float jogaccel_timer = 500;

float FreeTurn = 0.0625; // Used in motor velocity wrap determination
float SectorCounts = 0;
float Radians = 6.283185307;
float Sector_Angle = 1.047197551; // Radians, there are 6 sectors per complete cycle (2PI / 6)

// Motor Generics
float gain;
float c;
float m;
float Motor_Direction;
float Motor_PWM;
int tempread = 0;
double temp2 = 0;
//int sendme = 0;


// **********************************************************************
//  This loads the Axis with correct settings for proper values
void DriveAxisInitialise(struct MyAxis Axis) {
	Axis.Master_Command = 0;
	Axis.Status_Bits = 0x0;
	Axis.Command = 0;
	Axis.Scope_Me = 0;
	Axis.In_Setup_Mode = 0;
	Axis.Jog = 0;
// Set up the Drive motor
	Axis.Motor_Scale = 1.2f
			* (((Axis.Max_Speed / 360) * fabs(Axis.Ratio) * Axis.Motor_Counts)
					/ 1000);
	Axis.Com_Repeat = Axis.Motor_Counts / Axis.Pole_Pairs;
	SectorCounts = Axis.Com_Repeat / 6;
	Axis.Com_Radian_Scale = Radians / Axis.Com_Repeat;
	Axis.MotorLast_Inst_Pos = 0;
	Axis.Motor_Inst_Pos = 0;

	Axis.Com_Pos = 0;  // Radians!!!
	Axis.Motor_position = 0;
	Axis.MotorDemandPos = 0;
	Axis.System_position = 360;
	Axis.System_position = (Axis.System_position / Axis.Sys_Counts) * 360;
	Axis.SysDemandPos = Axis.System_position;
	Axis.SysDemand_Prev = Axis.SysDemandPos;
	Axis.SysActualVel = 0;
	Axis.Sys_follow_error = Axis.SysDemandPos - Axis.System_position;
	Axis.Motor_Enc_Vel_Prev = 0;
	Axis.Motor_Enc_Vel_Delta = 0;
	Axis.Duty_Cycle = 0;

}

// **********************************************************************
// This is used if using the Absolute motor encoder
void Motor_Absolute(struct MyAxis Axis) {



}




// **********************************************************************
// Read the system (output) Encoder
void Read_Output_Encoder(struct MyAxis Axis) {

}

// Update System Velocity from output encoder
void Output_Encoder_Vel(struct MyAxis Axis) {


}


// *****************************************************
// NOW PREPARE AND SEND DEMAND TO THE MOTOR
// Apply scale factor from user units to motor duty cycle
void DriveMotor(struct MyAxis Axis) {


}



// **************************************************
// The Scheduler makes sure servo and commutation activity runs in the correct order.
// Motor noise and control is very sensitive to changes in this order
// Servo must be completed first taking about 156us of the 500us available.
// About 344us will then be available to run the Admin part WHICH MUST NOT OVERRUN (in case you did not notice.. :) ).
void Servo_Scheduler(struct MyAxis Axis) {


		// Enable Pulse is needed to keep the MOSFETs enable alive and is to be toggled with less than 10ms intervals.
		//Enable_Pulse = 1;
		//Debug_LED3 = 1;
		//Read_Output_Encoder(Axis);  //
		//Debug_LED3 = 0;
		//UpdateDriveFeedback(Axis);  // 18us
		//Debug_LED3 = 1;

		 // NOTE:  When S ramping a fade this takes ~20us

		//Debug_LED3 = 0;  //27us without profiler, 57us with
		//DriveMotor(Axis);  // Audit and apply any kind of demand
		//M.Commutation(Axis); // Commutate the motor with a new demand
		// SopeTool can be called all the time but will only be active when debugging with serial port

		//ScopeTool(Axis);
		// Deal with external communications etc. - MUST NOT OVERRUN!!!!
		//Main_Admin();      // This is located in Main and is to support all other incidental functions.
		//Admin.Debug = Comms_M7.Send_From_M7(Axis.SysDemandPos);
		//Debug_LED3 = 1;
	  //  Comms_M7.Read_Comms(Admin.Debug);
		//Debug_LED3 = 0;
		//Enable_Pulse = 0;  // Turn off enable toggle

	// This @ 1KHz, used as a system clock tick:


}

// **********************************************************************
// This is used to enable or disable the axis safely
void EnableMe(struct MyAxis Axis, bool en) {
	if (en) {
		DriveAxisInitialise(Axis);
		Axis.Synced = false;
		Axis.Enable = true;
		Axis.Status_Bits = Axis.Status_Bits | ENABLED_BIT;
	} else {
		Axis.In_Setup_Mode = false;
		Axis.Enable = false;
		Axis.Status_Bits = (Axis.Status_Bits | ENABLED_BIT) ^ ENABLED_BIT;
		Axis.Jog = 0;
		//M.setDutyCycle(0);
		Axis.Synced = false;
	}
}

