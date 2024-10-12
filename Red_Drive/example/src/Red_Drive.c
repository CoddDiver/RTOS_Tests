/*
 * Sample Servo Drive
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

#include "UART_Manager.h"


#include <string.h>
#include <stdio.h>
#include <math.h>

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

char VelCopley_Template[] = "s r0xcb ";
char VelCopley[50];

bool LED_3_toggle = false;
bool LED4_Toggle = false;
bool GPIO34_Toggle = false;

int Drivepos = 0;

xTaskHandle APPLICATION_TASK_HANDLE;


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
	Board_LED_Set(3, true);
	Board_LED_Set(4, false);
	// GPIO 33, 34, 14 are available
	GPIO_Set(33, false);
	GPIO_Set(34, false);
	//GPIO_Set(14, false);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/


void Main_Wait(int ms) {
	vTaskDelay(configTICK_RATE_HZ * ms / 100);
}



void application(void *pvParameters) {
	//SetupIFTask();
//	Set_Loop_Tick_Time(Admin.Loop_Time_ms / 1000);  // For profiler.c
	//Set_Loop_Tick_Time(1);
	while (1) {
		GPIO_Set(33, toggle);
		toggle = !toggle;

		if (timer2 >= 2) {
			timer2 = 0;
			//netcheck();
		} else {
			timer2++;
		}

		P14 = GPIO_Get(14);

	//	AxisProfiler(Enable);

	//	Admin.Real_Time_ms = Admin.Real_Time_ms + Admin.Loop_Time_ms;

		vTaskDelay(configTICK_RATE_HZ / 100);
	}
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

	DEBUGOUT("Red Drive V1\r\n\r\n");

	//Admin_setup();
	// Initialise the Axis array
	//DriveAxisParameters(&Drive, COPLEY);

	xTaskCreate(application, (signed char* ) "Application",
			(unsigned short ) 256, NULL, (tskIDLE_PRIORITY + 1UL),
			(xTaskHandle* ) APPLICATION_TASK_HANDLE);



	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}

/**
 * @}
 */
