/*
 * Axis_Control.h
 *
 *  Created on: 30 Jan 2018
 *      Author: codda
 */

#ifndef AXIS_CONTROL_H_
#define AXIS_CONTROL_H_


#include "Axis_Configuration.h"
#include "Profiler.h"




void DriveAxisInitialise(struct MyAxis Axis);
void EnableMe(struct MyAxis Axis, bool en);
void Startup(float sampleTime);
void RunLoop();
void MotorControlLoop(struct MyAxis Axis);
void DriveMotor(struct MyAxis Axis);
void Servo_Scheduler(struct MyAxis Axis);

void ScopeTool(struct MyAxis Axis);
void UpdateDriveFeedback(struct MyAxis Axis);
void Read_Output_Encoder(struct MyAxis Axis);
void Output_Encoder_Vel(struct MyAxis Axis);
void Motor_QEI(struct MyAxis Axis);
void Motor_Absolute(struct MyAxis Axis);

#endif /* AXIS_CONTROL_H_ */
