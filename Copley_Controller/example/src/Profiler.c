/*
 * Profiler.cpp
 *
 *  Created on: 31 Jan 2018
 *      Author: codda
 */

#include "Axis_Configuration.h"
#include "Profiler.h"
#include "MyAdmin.h"
#include "board.h"
#include "Copley.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Added for Joystick Function

bool joy_OK = false;
float joy_offset = 0;
float joy_deadband = 0.001;
float joy_demand = 0;
float joy_decel = 0;
float joy_previous = 0;
float joy_limited_decel = 0;
float joy_accel_step = 1;

// Deal with profiling and scoping (generic)
int i = 1;
int printnow = 0;
// For MOTOR_SETUP
int settle_time = 4000;
bool pp0 = true;
bool pp1 = true;
bool pp2 = true;
bool pp3 = true;

// Default Profile attributes
int accel_t = 1000;
int settle_t = 3000;
int test_t = 1000;
int decel_t = 1000;
int end_t = 1000;
int jogaccel_time = 100;

float dwell = 8000;

// For S Ramps
float D = 10; // Fade distance
float T = 15; // Fade time
float ts; // The actual value used after sanity checks(S ramping)
float tsfade = 0.6667; // segment time for S ramped fade
float tscut = 0.33; // segment time for S ramped cut
float j; // jerk value for fade
float aj; // accel value when S ramping
float s = 0;  // Profiler internal demand speed (always positive)
float Vp;  // Top speed for the move Deg/s @ load (256:1 Ratio to motor)
float Va; // mid speed point
float as; // acceleration value during linear accel stage
float Vj; // Speed at bottom intersection
float Vjp; // Speed at top intersection
float co = 0; // constant for up ramp y=mx+c
float b; // constant for down ramp y = mx+b
double LOOPs = 0.005;
bool setup = false;
double fadetime;

extern char VelCopley_Template[];
extern char VelCopley[];
extern char Veldem[];
bool Soak_dir = false;  // Flag for soak direction

float integrator[5] = { 0 };

void Set_Loop_Tick_Time(float _s) {
	LOOPs = _s;
}

// **********************************************************************
// An integrator, used for Integral gain
float Integrator(float _val) {
	int i = 0;
	float sum = 0;

	for (i = 0; i == 3; i = i + 1) {
		integrator[i] = integrator[i + 1];
		sum = sum + integrator[i];
	}
	integrator[4] = _val;
	sum = sum + _val;
	return sum;
}

// **********************************************************************
// Safely complete a profiled event
void End_me(int _soft) {
	// Drive.Enable = false;
	if (_soft == 1){
		Drive.Command = M4_JOYSTICK;
	} else {
		Drive.Command = OFF;
	}
	Drive.sysdojog = 0;
	Drive.Jog = 0;
	Drive.Run_Count_Jog = 0;
	setup = false;
	DEBUGOUT("End_me Completed");
}

// **********************************************************************
//  Pre-calculate parameters required for an S ramped move
void Profile_SRamp() { // For S ramped movement using Servo for S ramping
	DEBUGOUT("Setup Fade\n\r");
	//Drive.System_position = Drive.SysDemandPos;

	if ((Drive.Fade_Target >= Drive.Min_Limit)
			& (Drive.Fade_Target <= Drive.Max_Limit)) {
		D = Drive.Fade_Target - Drive.SysDemandPos; // Calculate distance to move
	} else {
		D = 0;
		//	pc.printf("\n\r Move Outside Limits");
	}

	if (D < 0){
		D = D * -1;
		Drive.sysjogdir = -1;
	} else {
		Drive.sysjogdir = 1;
	}

	//D = 100;
	tsfade = Drive.Fade_Accel_Time / 3;  // segment time for S ramped fade
	tscut = Drive.Cut_Accel_Time / 3;

	if (Drive.Fade_Time <= (6 * tsfade + 0.2)) {
		ts = tscut;
		T = Drive.Fade_Time;
	} else {
		ts = tsfade;
		T = Drive.Fade_Time;
	}
	if (Drive.Fade_Time <= (6 * tscut + 0.2)) {
		T = 6 * tscut + 0.2;  //min fade time
	}

	Vp = D / (T - (3 * ts));           // Equation 1
	if (Vp > Drive.Max_Speed) {         //Check for maximum speed condition
		Vp = Drive.Max_Speed;           //Do the fade as fast as possible
		T = (D + (Vp * (3 * ts))) / Vp;
	}

	// New version based on S-Ramping Doc - V2

	j = Vp / (2 * ts * ts);
	as = j * ts;
	co = -(Vp / 4);
	s = 0;
	fadetime = 0;

	//	if (abs(Vp) <= Drive.Fade_Min_Speed) {
	//pc.printf("\n\r Invalid Fade");
	//	End_me(Drive);
	//} else {
	setup = true;
	DEBUGOUT("Setup Fade Done\n\r");
	//}

}

void M4_Joystick() {
	//joy_demand = (Drive.Joy_Demand - joy_offset) * Drive.Joy_SF;
	joy_demand = Drive.Joy_Demand / Drive.Joy_SF;
	//joy_demand = joy_demand / 20;

	if (joy_demand <= -(joy_deadband )) {
		Drive.sysjogdir = -1;
		Drive.Jog = -(joy_demand + joy_deadband);
	} else if (joy_demand >= joy_deadband) {
		Drive.sysjogdir = 1;
		Drive.Jog = joy_demand - joy_deadband;
	} else {
		Drive.Jog = 0;
	}
/*
	Drive.Jog = joy_demand / 10000;
	if (Drive.Jog >= 0){
		Drive.sysjogdir = 1;
	} else {
		Drive.sysjogdir = -1;
		Drive.Jog = Drive.Jog * -1;
	}
*/
	if (Drive.Jog <= joy_previous) { // Slowing down
		joy_limited_decel = joy_previous - joy_decel;
		if (Drive.Jog < joy_limited_decel) {
			Drive.Jog = joy_limited_decel;
		}
	} else { // Speeding up
		joy_decel = Drive.Jog / Drive.Joy_Decel_Time; //Calculate the decel rate ready for deceleration phase

		joy_accel_step = (Drive.Max_Speed / 3000);

		if ((Drive.Jog - joy_previous) >= joy_accel_step) { // Limit the acceleration
			Drive.Jog = Drive.Jog + joy_accel_step;
		}
	}

	if (Drive.Jog < 0) {
		Drive.Jog = 0;
	}
	if ((Drive.System_position >= Drive.Max_Limit)
			|| (Drive.System_position <= Drive.Min_Limit)) {
		Drive.Jog = 0;
	}

	joy_previous = Drive.Jog;
	Drive.sysdojog = (Drive.Jog * 1000 * (float) Drive.sysjogdir);

	Drive.SysDemandPos = Drive.SysDemandPos + (Drive.Jog * Drive.sysjogdir);
}

void M4_Fade() {
	if (!setup) {
		Profile_SRamp();
	}

	if (setup) {
		if ((fadetime < ts) & (s < Vp)) {
			s = (j / 2) * fadetime * fadetime;  //bottom parabola
			fadetime = fadetime + LOOPs; // This provides the base time for the fade sequence
		} else if ((fadetime >= ts) & (fadetime < (2 * ts))) {
			s = (as * fadetime) + co; //steady accel stage
			fadetime = fadetime + LOOPs;
		} else if ((fadetime >= (2 * ts)) & (fadetime < (3 * ts))) {
			s = (-(j / 2) * (fadetime - (3 * ts)) * (fadetime - (3 * ts))) + Vp; // Top parabola
			fadetime = fadetime + LOOPs;
		} else if ((fadetime >= (3 * ts)) & (fadetime < (T - (3 * ts)))) {
			s = Vp;  // Steady Speed Stage
			fadetime = fadetime + LOOPs;
		} else if ((fadetime >= (T - (3 * ts))) & (fadetime < (T - (2 * ts)))) {
			s = (-(j / 2) * (fadetime - (T - (3 * ts)))
					* (fadetime - (T - (3 * ts)))) + Vp; // Top parabola down
			fadetime = fadetime + LOOPs;
		} else if ((fadetime >= (T - ts - ts)) & (fadetime < (T - ts))) {
			s = -as * (fadetime - T) + co; //steady decel stage
			fadetime = fadetime + LOOPs;
		} else if ((fadetime >= (T - ts)) & (s < Vp) & (fadetime <= T)) {
			s = (j / 2) * (T - fadetime) * (T - fadetime); //bottom parabola to end
			fadetime = fadetime + LOOPs;
		} else if ((fadetime >= T) & (fadetime <= (T + 2))) {
			s = 0;
			fadetime = fadetime + LOOPs;
		} else if (fadetime > (T + 2)) {
			Drive.Scope_Me = false;
			setup = 0;
			End_me(1);
		}
		// compute the new velocity and position demand:
		Drive.sysdojog = s *  Drive.sysjogdir;
		if ((T - fadetime) >= 0) {
			Drive.Fade_Run_Time = (T - fadetime);
		} else {
			Drive.Fade_Run_Time = 0;
		}

		Drive.SysDemandPos = Drive.SysDemandPos + (Drive.sysdojog * LOOPs);

		//Drive.M7_Pos_Target = Drive.SysDemandPos + (Drive.sysdojog * LOOPs);
		//M7_Pos_Follow();

	}
}

void M4_Soak() {
	if ((Drive.Command != M4_FADE) && Drive.Enable) {
		Admin.Soak_Done += 1;
		//pc.printf("\n Soak Done = %d out of %d", Admin.Soak_Done,
		//Admin.Soak_Required);
		if (Admin.Soak_Done <= Admin.Soak_Required) {
			if (Drive.Kp_Sys == 0) {
				if (Soak_dir) {
					D = Drive.Soak_Target1; //For soaking without system encoder
					Soak_dir = false;
				} else {
					D = -Drive.Soak_Target1; //For soaking without system encoder
					Soak_dir = true;
				}
			} else {
				if (Soak_dir) {
					Drive.Fade_Target = Drive.Soak_Target1;
					Soak_dir = false;
				} else {
					Drive.Fade_Target = Drive.Soak_Target2;
					Soak_dir = true;
				}
			}

			Drive.Command = M4_FADE;
		} else {
			Drive.Master_Command = OFF;
			//pc.printf("\n\r Soak Done = %d, End", Admin.Soak_Done);
			Admin.Soak_Done = 0;
		}
	}
}

void M7_Pos_Follow() {

	if (fabs(Drive.System_position - Drive.M7_Pos_Target)
			<= Drive.Sys_follow_limit) {
		Drive.SysDemandPos = Drive.M7_Pos_Target;
	} else {
		Drive.SysDemandPos = Drive.System_position;
		Drive.Status_Bits = Drive.Status_Bits | P_FOLLOW_BIT;
	}

}

// **********************************************************************
// AXIS PROFILER TO RUN AT 1 kHz
// Depending on the value of Drive.Command different things will happen ////
void AxisProfiler() {
	if (Drive.Command == OFF) {
		//Drive.SysProfilerVel = 0;
		setup = 0; // This prevents the fade playing out that remaining if a system disable / enable cycle interrupts a fade.
	}
	// M4_SOAK:  Run a soak Test via managing the M4_FADE function.  This is very useful in development.
	// would be bypassed by M7.
	if (Drive.Master_Command == M4_SOAK) {
		M4_Soak();
		//Drive.Advance_Angle = 100;
	}

	// M4_FADE:  Very useful for development, would be bypassed by M7
	if (Drive.Command == M4_FADE) {
		M4_Fade();
	} else {
		setup = 0; // This prevents the fade playing out that remaining if a system disable / enable cycle interrupts a fade.
	}

	if (Drive.Command == M7_POS_FOLLOW) {
		M7_Pos_Follow();
	}

// M7 parent control, Joy_demand is in Deg/s in this case.
// M7 is responsible for safe profiling.
	if (Drive.Command == M7_JOG) {
		Drive.sysdojog = Drive.Joy_Demand;
		Drive.SysDemandPos = Drive.SysDemandPos + (Drive.Joy_Demand / 1000);
	}

// M7 parent control, Joy_demand is in Deg/s in this case.
// M7 is responsible for safe profiling.
	if (Drive.Command == M7_POS_FOLLOW) {
		Drive.sysdojog = Drive.Joy_Demand;
		Drive.SysDemandPos = Drive.SysDemandPos + (Drive.Joy_Demand / 1000);
	}

// JOYSTICK:  This has been useful with a stand-alone drive.
	if (Drive.Command == M4_JOYSTICK) {
		M4_Joystick();
	}

	if (Drive.Command == MOTOR_SETUP) {
		//Motor_Setup();
	}

	Pos_Demand_Generator();

}

void Pos_Demand_Generator() {
// The profiler generates a position demand which must be processed and sent to the motor loop as a velocity demand
// **********************************************************
// Generate perfect velocity demand for the System:
	Drive.SysProfilerVel = Drive.SysDemandPos - Drive.SysDemand_Prev;
	Drive.SysDemand_Prev = Drive.SysDemandPos;
// **********************************************************

//  Calculate follow error:
	Drive.Sys_follow_error = Drive.SysDemandPos - Drive.System_position;

// Apply proportional and feed forward gain to the System Velocity demand
	Drive.SysDerivedVel = ((Drive.SysProfilerVel * Drive.Kff_Sys)
			+ (Drive.Sys_follow_error * Drive.Kp_Sys));

// Apply Integral Gain to the System Velocity demand
	Drive.SysDerivedVel = Drive.SysDerivedVel
			+ (Integrator(Drive.Sys_follow_error) * Drive.Ki_Sys);

	Drive.SysUnit_Sec = Drive.SysDerivedVel * (1/LOOPs);
	//Drive.System_position = Drive.SysDemandPos;
}

