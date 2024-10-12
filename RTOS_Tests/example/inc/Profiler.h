/*
 * Profiler.h
 *
 *  Created on: 31 Jan 2018
 *      Author: codda
 */

#ifndef PROFILER_H_
#define PROFILER_H_


#include "Axis_Configuration.h"



    // This is to be run at 1 KHz ONLY
    void AxisProfiler();


    void Set_Loop_Tick_Time(float _s);
    void End_me();
    float Integrator(float _val);
    void Profile_SRamp();
    void M7_Pos_Follow();
    void M4_Soak();
    void M4_Fade();
    void Motor_Setup();
    void M4_Joystick();
    void Pos_Demand_Generator();

#endif /* PROFILER_H_ */
