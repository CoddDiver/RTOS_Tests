/*
 * Main Application for Network Stepper
 *
 */

#include "lwip/init.h"
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/memp.h"
#include "lwip/tcpip.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/timers.h"
#include "board.h"
#include "Network_Link.h"
#include "TCP_Connection.h"
#include "myprint.h"

#include "MyAdmin.h"
#include "Axis_Configuration.h"
#include "Profiler.h"
#include "UART_Manager.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <Stepper.h>

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

extern void Servo_Scheduler(struct MyAxis Axis);

extern char READ[50];
extern char NEMA[50];
int go = 1;
char checkstring[15];
int timer = 0;
int timer2 = 50;
int broken = 0;
int fail = 0;
extern struct MyAxis Drive;
extern struct MyAdmin Admin;
extern int flag;
extern int send;
extern char InBound[100];
extern char OutBound[100];
char outpan[10];
char outsyspos[10];
char outtime[10];
char outspeed[10];
char outerror[10];
char panvel[10];
char header[10]; //= "real ";

int Enable = 0;
int Stopped = 0;
int counter = 0;
int oldcounter = 0;
int pan, tilt, duration, jpan, jtilt, command, newkp, newvff;
char valid[4];
char char_counter[10];
char value[10];
int enablecount = 0;
int sendtimer = 0;
int sendcount = 0;

bool LED_3_toggle = false;
bool LED4_Toggle = false;
bool GPIO34_Toggle = false;

int Drivepos = 0;
xTaskHandle STEPPER_TASK_HANDLE;
xTaskHandle APPLICATION_TASK_HANDLE;
xTaskHandle TCP_TASK_HANDLE;

typedef struct _data {
	int pan;
	int tilt;
	int time;
	int speed;
	int error;
} data;
data real;

typedef struct _data2 {
	float pan;
	float tilt;
	float time;
} data2;
data2 fade;
data2 joy;

bool toggle = false;
bool toggle2 = false;
bool P14 = false;

const char space[2] = " ";
char *token;
char test[100];
//char res[20];

int speed = 100;

/* Sets up system hardware */
static void prvSetupHardware(void) {
	SystemCoreClockUpdate();
	Board_Init();

	/* LED0 is used for the link status, on = PHY cable detected */
	/* Initial LED state is off to show an unconnected cable state */
	Board_LED_Set(1, false);
	Board_LED_Set(2, false);
	Board_LED_Set(3, false);
	Board_LED_Set(4, false);

	GPIO_Set(33, false);
	GPIO_Set(34, false);

	GPIO_Set(27, false);
	GPIO_Set(28, false);
	GPIO_Set(29, false);
	GPIO_Set(30, false);

	//GPIO_Set(14, false);  14 is an input
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/
void Enable_Drive(void) {
	Drive.Command = M4_JOYSTICK;

	sprintf(header, "%s", "Enable");
	Drive.SysDemandPos = Drive.System_position;

	Stepper.Case_Status = COP_TRY_ENABLE;

}

void Fade(void) {
	Drive.Fade_Target = pan;
	Drive.Fade_Time = duration;
	Drive.Command = M4_FADE;
	DEBUGOUT("Calling Fade\n\r");
}

void Joystick(void) {
	if (Enable == 1) {
		Drive.Joy_Demand = jpan;
		//	MYDEBUG(0, Drive.SysProfilerVel, 0,1);
		//	DEBUGOUT("  %d \r\n", jpan);
	} else {
		Drive.Joy_Demand = 0;
	}
}

void Main_Wait(int ms) {
	vTaskDelay(configTICK_RATE_HZ * ms / Admin.TICK_RATE_HZ_div);
}

void BreakIn() {

	strcpy(checkstring, "1234");
	go = strncmp(checkstring, InBound, 4); // Check this will parse correctly

	if (go == 0) {
		//broken = 1;
		//sscanf(InBound, "%d %d %d %d %d %d %d %d %d", &valid, &counter, &command, &pan
		//	&tilt, &duration, &jpan, &jtilt, &newkp, &newvff);
		token = strtok(InBound, space);
		strcpy(valid, token);
		token = strtok(NULL, space);
		strcpy(char_counter, token);
		counter = atoi(char_counter);

		if (counter != oldcounter) {
			broken = 1;
			oldcounter = counter;
			// Strip out the rest of the message
			token = strtok(NULL, space);
			strcpy(value, token);
			command = atoi(value);
			token = strtok(NULL, space);
			strcpy(value, token);
			pan = (atoi(value) / 1000);
			//MYDEBUG("0", 0, pan, 2);
			token = strtok(NULL, space);
			strcpy(value, token);
			tilt = (atoi(value) / 1000);
			//MYDEBUG("0", 0, tilt, 2);
			token = strtok(NULL, space);
			strcpy(value, token);
			duration = atoi(value);
			//MYDEBUG("0", 0, duration, 2);
			token = strtok(NULL, space);
			strcpy(value, token);
			jpan = atoi(value);
			token = strtok(NULL, space);
			strcpy(value, token);
			jtilt = atoi(value);
			token = strtok(NULL, space);
			strcpy(value, token);
			newkp = atoi(value);
			token = strtok(NULL, space);
			strcpy(value, token);
			newvff = atoi(value);

			switch (command) {
			case 0:
				DEBUGOUT("DUD\r\n");
				break;
			case 1: //Fade
				if (Enable == 1) {
					sprintf(header, "%s", "Moving");
					Fade();
				}
				break;
			case 3:  // Enable
				enablecount++;
				Enable_Drive();

				break;
			case 10: //Polling
				if (Enable == 1) {
					sprintf(header, "%s", "Enabled");
				} else {
					sprintf(header, "%s", "Disabled");
				}
				Joystick();
				break;
			case 90:  //Stop
				sprintf(header, "%s", "Stopped");
				End_me(0);
				Drive.SysProfilerVel = 0;
				Enable = 0;
				Stepper.Case_Status = COP_TRY_DISABLE;
				break;
			case 20:  //Change gain
				sprintf(header, "%s", "GAIN_UPDATED");
				Drive.Kp_Sys = newkp;
				Drive.Kp_Sys = Drive.Kp_Sys / 1000;

				Drive.Kff_Sys = newvff;
				Drive.Kff_Sys = Drive.Kff_Sys / 1000;

				//MYDEBUG(0, Drive.Kp_Sys, 0, 1);
				//DEBUGOUT("\r\n");
				break;
			default:
				DEBUGOUT("GARBAGE\r\n");
			}
		} else {
			//command = -1; //Flag the failure
			strcpy(InBound, "\0");
			fail++;
			DEBUGOUT("%d\r", fail);
		}
	}
}

void BuildOut() {
	sendcount++;
	OutBound[0] = '\0';
	ftoa((Drive.SysUnit_Sec), panvel, 3);
	ftoa((Drive.SysDemandPos), outpan, 3);
	ftoa((Drive.Fade_Run_Time), outtime, 3);
	ftoa((Drive.Sys_follow_error), outerror, 3);
	ftoa((Drive.System_position), outsyspos, 3);
	//Drivepos =  (Drive.sysdojog * 1000);
	//sprintf(outpan, "%d", (Drivepos));
	//sprintf(outtime, "%d", Drivepos);
	sprintf(outspeed, "%d", 0);
	//sprintf(outerror, "%d", Drive.Sys_follow_error);

	strcpy(OutBound, header);
	strcat(OutBound, space);
	strcat(OutBound, outpan);
	strcat(OutBound, space);
	strcat(OutBound, outsyspos);
	strcat(OutBound, space);
	strcat(OutBound, outtime);
	strcat(OutBound, space);
	strcat(OutBound, panvel); //outspeed
	strcat(OutBound, space);
	strcat(OutBound, outerror);
	strcat(OutBound, space);
	strcat(OutBound, panvel);
	strcat(OutBound, space);
}

void Stepper_Manager(void *pvParameters) { // Task to look after the Copley.

	Stepper.Case_Status = COP_STARTING;
	while (1) {
		while (Stepper.Response != COPLEY_ERROR) {
			// Will need an external keep alive is case of no responses at all!
			vTaskDelay(configTICK_RATE_HZ / (Admin.TICK_RATE_HZ_div));

			switch (Stepper.Case_Status) {

			case COP_STARTING:

				DEBUGOUT("COPLEY CONFIGURED\r\n");
				Stepper.Case_Status = COP_DISABLED;

				break;
			case COP_DISABLED:
				Stepper_Get_Pos();
				Drive.System_position = Stepper.Position;
				Drive.System_position = Drive.System_position / Drive.Ratio;
				// Wait for parent application to enable the drives using state COP_TRY_ENABLE
				break;
			case COP_TRY_ENABLE:
				//Application must check position profiler is clean for startup

				Enable = 1;
				break;
			case COP_ENABLED:

				Board_LED_Set(3, LED_3_toggle);
				LED_3_toggle = !LED_3_toggle;
				if (Enable == 1) {
					Stepper.VelDemand = Drive.SysUnit_Sec * Drive.Ratio;
					Stepper_Send_Demand(Stepper.VelDemand);
				}
				Stepper_Get_Pos();
				Drive.System_position = Stepper.Position;
				Drive.System_position = Drive.System_position / Drive.Ratio;
				//DEBUGOUT("Pos = %d\r", Copley.Position);

				//vTaskDelay(configTICK_RATE_HZ / (Admin.TICK_RATE_HZ_div));
				break;
			case COP_TRY_DISABLE:


				break;
			case COP_FAULT:

				break;
			default:
				DEBUGOUT("DROPPED OFF COPLEY STATE MACHINE");
			}
		}
		DEBUGOUT("******** COPLEY RECOVERY PROGRAM *************");
		// Should call any system level kill code as required, or delegate to parent on COP_FAULT.
		End_me(0);
		Drive.SysProfilerVel = 0;
		Enable = 0;
		Stepper.Case_Status = COP_FAULT;  // Parent must restart as required.
		Stepper.Response = COPLEY_UNKNOWN; // Re-Enter case state machine until next Copley Response update
	}

}

void TCP_Manager(void *pvParameters) {
	SetupIFTask();
	while (1) {

		tcp_read();
		BreakIn();
		if ((broken >= 1)) {
			GPIO_Set(34, GPIO34_Toggle);
			broken = 0;
			BuildOut();
			tcp_send(OutBound);
			GPIO34_Toggle = !GPIO34_Toggle;
		}

		vTaskDelay(configTICK_RATE_HZ / (Admin.TICK_RATE_HZ_div * 2));
	}
}

void application(void *pvParameters) {
	//SetupIFTask();
	Set_Loop_Tick_Time(Admin.Loop_Time_ms / 1000);  // For profiler.c
	//Set_Loop_Tick_Time(1);
	while (1) {
		GPIO_Set(33, toggle);
		toggle = !toggle;

		if (timer2 >= 2) {
			timer2 = 0;
			netcheck();
		} else {
			timer2++;
		}

		P14 = GPIO_Get(14);

		AxisProfiler(Enable);

		Admin.Real_Time_ms = Admin.Real_Time_ms + Admin.Loop_Time_ms;

		vTaskDelay(configTICK_RATE_HZ / Admin.TICK_RATE_HZ_div);
	}
}

// ************************************************************************
// Admin data is used to drive the Round Robin
void Admin_setup() {
	Admin.Real_Time_ms = 0;
	Admin.Loop_Time_ms = 10;
	Admin.TICK_RATE_HZ_div = (1000 / Admin.Loop_Time_ms); // Ticker interval for Round Robin
	//Admin.Counts_to_Commutation = 1; //
	Admin.Master_Counter = 1;
	Admin.Master_Sub_Counter = 1;
	Admin.Soak_Required = 2;
	Admin.Soak_Done = 0;
	Admin.Debug = 0;
}

/**
 * @brief	main routine for example_lwip_tcpecho_freertos_17xx40xx
 * @return	Function should not exit
 */
int main(void) {
	prvSetupHardware();

	/* Setup DAC pins for board and common CHIP code */
	Chip_DAC_Init(LPC_DAC);
	Setup_UART_SELECTION(9600);

	DEBUGOUT("STEPPER MOTION CONTROLLER V1\r\n\r\n");

	Admin_setup();
	// Initialise the Axis array
	DriveAxisParameters(&Drive, COPLEY);

	xTaskCreate(application, (signed char* ) "Application",
			(unsigned short ) 256, NULL, (tskIDLE_PRIORITY + 1UL),
			(xTaskHandle* ) APPLICATION_TASK_HANDLE);

	xTaskCreate(Stepper_Manager, (signed char* ) "Copley_Manager",
			(unsigned short ) 256, NULL, (tskIDLE_PRIORITY + 1UL),
			(xTaskHandle* ) STEPPER_TASK_HANDLE);

	xTaskCreate(TCP_Manager, (signed char* ) "TCP_Manager",
			(unsigned short ) 256, NULL, (tskIDLE_PRIORITY + 1UL),
			(xTaskHandle* ) TCP_TASK_HANDLE);

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}

/**
 * @}
 */
