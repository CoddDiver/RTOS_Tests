/*
 * Copley.h
 *
 *  Created on: 21 Jul 2020
 *      Author: Andrew
 */

#ifndef INC_COPLEY_H_
#define INC_COPLEY_H_

#define UNKNOWN 0
#define COPLEY_OK 1
#define COPLEY_ERROR 2
#define POSITION 3

struct MyCopley {
	int Enabled;
	int Read_Status;
	int Dir_Setting;
	int Response;
	int Position;  // Encoder counts
	int VelDemand; // Velocity in 1 counts/s
} Copley;

int readCopleyResponse(char *ToRead);
void Copley_Disable(void);
void Copley_Enable(void);
void Copley_Configure();
void Copley_Read(int _why);
void Copley_Send_Demand(int _speed);
void Set_Direction(int _dir);
void Copley_Get_Pos(void);
void Copley_Wait(int ms);
#endif /* INC_COPLEY_H_ */
