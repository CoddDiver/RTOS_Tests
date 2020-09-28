/*
 * Axis_Configuration.h
 *
 *  Created on: 31 Jan 2018
 *      Author: codda
 */

#ifndef AXIS_CONFIGURATION_H_
#define AXIS_CONFIGURATION_H_

//#include "mbed.h"
#include "board.h"


// defines for mytype declaration in configuration call
#define COPLEY 10

// defines for Axis Command
#define M4_FADE 3
#define M7_JOG 4      // Joystick demand from M7
#define M7_POS_FOLLOW 5  // Rolling Position Demand from M7
#define M4_JOYSTICK 6
#define M4_SOAK 7
#define MOTOR_SETUP 8
#define OFF 0

#define DRAG 100
#define ASSIST 101
#define ENABLED_BIT 8
#define CURRENT_BIT 4
#define FOLLOW_BIT 2
#define P_FOLLOW_BIT 1


// Axis Specific structure
struct MyAxis {
	int Master_Command;
	int Command;
	uint8_t Status_Bits;			// 0x8 = Enabled, 0x4 = Current Fault,  0x2 = Follow Error, 0x1 = ?
	int Sys_Counts;
	float Max_Speed;
	float Joy_Demand;
	int Joy_Decel_Time;
	int Joy_SF; 				// Scales analogue joystick value into counts/ms
	bool Enable;
	int Comtype;
	float Jog;          		// System Jog demand speed in Deg/profiler tick
	float sysjogdir;
	double ramp;
	int Run_Count_Jog;
	float Run_Count;
	float Ratio;
	float Max_Limit;       			// System Position Pos limit in Deg
	float Min_Limit;				// System Position Neg limit in Deg
	float System_position;  		// System position in Deg
	float System_position_prev;  	// Previous position in Deg
	double System_position_offset;  // Value to zero the System encoder in Deg
	double Sys_follow_error;  		// System follow error in Deg
	float SysDemandPos;  			// System Demand position in Deg
	double SysProfilerVel;  		// System profiler velocity in Deg/profiler tick
	double SysDerivedVel; 			// System profiler demand velocity in Deg/profiler tick
	float SysUnit_Sec;				// System demand speed in units/sec
	double SysActualVel;  			// System actual velocity in Deg/s
	double SysDemand_Prev; 			// Previous System actual velocity in Deg/profiler tick
	int Direction;
	float Motor_Scale;
	double Motor_final;
	float Kp_Vel;
	float K_Assist;
	float Drag;
	float Kp_MotPos;
	float Kp_Sys;
	float Ki_Sys;  					// System integration gain
	float K_Diff; 					// Differential gain
	float Kff;
	float Kff_Sys;
	double Follow_Error;  			// Motor follow error in motor counts
	float Sys_follow_limit;  		// Follow limit of system in Deg
	float Feed_Forward;
	int Motor_Inst_Pos;  			// Position in motor encoder counts
	double MotorDemandPos;  		// Demand position in motor counts
	int MotorLast_Inst_Pos;
	float Motor_RPM;				// Motor RPM
	float Motor_Enc_Vel;			// encoder counts / servo tick
	float Fade_Time;				// s
	float Fade_Run_Time;
	float Fade_Target;   			// Deg
	double Soak_Target1;   			// Deg
	double Soak_Target2;   			// Deg
	float Fade_Speed;				// Fade Speed Demand in Deg/s
	float Fade_Min_Speed;			// Minimum speed Allowed in a fade
	float Fade_Accel_Time;			// Time in s for accel / decel fades
	float Cut_Accel_Time; 			// Time in s for accel / decel cuts
	float Motor_Enc_Vel_error;
	float Motor_Enc_Vel_Prev;
	float Motor_Enc_Vel_Delta;
	float Motor_position;			// Motor Position, Counts
	float dojog;  					// Velocity demand Deg/ms (Profiler tick)
	float sysdojog;  				// Velocity demand Deg/s
	float M7_Pos_Target;			// Absolute Position Follow demand from Parent Control, Deg.
	int Motor_Wrap;
	float Motor_Counts;
	int Motor_Offset;
	int Com_Offset;
	int Block_sector;
	int Sector;
	int Previous_Sector;
	int Pole_One; 					// raw encoder value when POLE 1 is energised
	int Pole_Pos; 					// register for virtual hall derivation
	int Enc_Pos;
	int Enc_Pos_Prev;
	float Com_Pos;
	float Com_Repeat;
	float Com_Radian_Scale;
	float Com_Angle;
	int Pole_Pairs;
	float Advance_Angle;
	float Advance_Angle_Adjustment;
	float Max_Dynamic_Power;
	float Dynamic_Power;
	bool flip_sin;
	bool flip_halls;
	float A_sin;
	float B_sin;
	float C_sin;
	float AA_sin;
	float BB_sin;
	float CC_sin;
	float curr1;
	float curr2;
	float curr3;
	bool Synced;
	bool In_Setup_Mode;
	bool Scope_Me;
	float Duty_Cycle;
}Drive;


void DriveAxisParameters(struct MyAxis *Axis, int mytype);

#endif /* AXIS_CONFIGURATION_H_ */
