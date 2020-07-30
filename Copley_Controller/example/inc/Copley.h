/*
 * Copley.h
 *
 *  Created on: 21 Jul 2020
 *      Author: Andrew
 */

#ifndef INC_COPLEY_H_
#define INC_COPLEY_H_

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


struct MyCopley {
	int Case_Status;
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
void Copley_Set_Pos(float _pos);
void Copley_Wait(int ms);
#endif /* INC_COPLEY_H_ */
