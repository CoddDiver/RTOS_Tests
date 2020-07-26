/*
 * Main Application for Network Controllers
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
//#include "ESP8266.h"
#include "http_messages.h"

#include <string.h>
#include <stdio.h>

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
extern void http_server_netconn_serve(void);

//extern void DriveAxisParameters(struct MyAxis Axis);
//extern struct MyAxis Drive;
extern char READ[128];
extern char NEMA[256];
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
extern char stuf2[256];
char outpan[10];
char outsyspos[10];
char outtime[10];
char outspeed[10];
char outerror[10];
char panvel[10];
char header[10]; //= "real ";
//char space[] = " ";
int DoMove = 0;
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

int ESPstage = 0;
bool LED_3_toggle = false;
int waitESP = 0;

char snd[100];
bool printflag = false;
int Drivepos = 0;

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
char res[20];
int Copley_Response;
extern int bufferWritingIndex;
char serverIp[16];
int DataLength = 0;
extern int htmlflag;

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
	// GPIO 33, 34, 14 are available
	GPIO_Set(33, false);
	GPIO_Set(34, false);
	//GPIO_Set(14, false);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/
void Fade(void) {

	Drive.Fade_Target = pan;
	Drive.Fade_Time = duration;

	Drive.Command = M4_FADE;
	DEBUGOUT("Fade\n\r");
	//Profile_SRamp();
	//AxisProfiler();
	//DEBUGOUT("Fade = %d", Drive.Sys_Counts);
}

void mywait(void) {

	while (waitESP <= 10) {
		waitESP++;
	}
}

void Copley_Configure() {

	Send_UART("AT+RST\r\n");
	while (Copley_Response != READY) {
		Poll_UART();
	};
	DEBUGOUT("Reset success\n");

	//Send_UART("ATE0\r\n");//disable ECHO
	// tempEspStatus = 0;
	//while(tempEspStatus!=OK){Poll_UART();};
	Send_UART("AT+CWMODE=1\r\n");	    //enable station mode
	Copley_Response = 0;
	while (Copley_Response != COPLEY_OK) {
		Poll_UART();
	};
	//Send_UART("AT+CWMODE_CUR=1\r\n");	    //enable station mode
	//tempEspStatus = 0;
	//while (tempEspStatus != OK) {
	//	Poll_UART();
	//};
	//Send_UART("AT+CWHOSTNAME=\"MAPLE_VILLA\"\r\n");	    // set host name
	//tempEspStatus = 0;
	//while (tempEspStatus != OK) {
	//	Poll_UART();
	//};

	Send_UART("AT+CIPMUX=1\r\n");	    // enable multiple connections
	Copley_Response = 0;
	while (Copley_Response != COPLEY_OK) {
		Poll_UART();
	};

	Send_UART("AT+CIPSERVER=1,80\r\n");	    //start server at port 80
	Copley_Response = 0;
	while (Copley_Response != COPLEY_OK) {
		Poll_UART();
	};

	//Send_UART("AT+CIPSTO=5\r\n");//Server timeout=5 seconds
	//while(tempEspStatus!=OK){Poll_UART();};

	DEBUGOUT("Connecting to WiFi..");
	Send_UART("AT+CWJAP_CUR=\"EE-Hub-hX7z\",\"BAR-gave-lunar\"\r\n");
	// char responseFromEsp;
	Copley_Response = 0;
	while (1) {
		Poll_UART();
		//    responseFromEsp=tempEspStatus;

		if (Copley_Response == COPLEY_OK || Copley_Response == FAIL)
			break;
	}

	if (Copley_Response == COPLEY_OK) {
		DEBUGOUT("Connected to Wifi\n");

		// __disable_irq();
		Send_UART("AT+CIFSR\r\n");	  //check obtained IP
		//   esp.scanf("+CIFSR:STAIP,\"%[^\"]",&serverIp);//+CIFSR:STAIP,"192.168.8.109"
		// __enable_irq();
		Copley_Response = 0;
		while (Copley_Response != COPLEY_OK) {
			Poll_UART();
		};
	}
}

int Copley_ConnectionManager() {
	DEBUGOUT("AND AGAIN");
	htmlflag = 0;
	while (Copley_Response != ZERO_CONNECT) {
		Poll_UART();
	};
	while (htmlflag <= 100) {
		Poll_UART2();
	};  // Wait until there is a connection
	//DEBUGOUT("WAITING");

	waitESP = 0;
	while (waitESP <= 2) {
		Poll_UART();
	}

	DEBUGOUT("SENDING");
	DataLength = strlen(http_index_html) + strlen(http_html_hdr);
	//Send_UART("AT+CIPSEND=1,211\r\n");
	Send_UART("AT+CIPSEND=0,112\r\n");

	while (1) {
		Poll_UART();
		if (Copley_Response == COPLEY_OK) {
			break;
		}
		if (Copley_Response == ESPERROR) {
			return 0;
		}
	}

	waitESP = 0;
	while (waitESP <= 1) {
		Poll_UART();
	}
	if (htmlflag == 900) {
		Send_UART(
				"<html><head><title>Control Interface</title></head><body><h1>LED 4 is now ON!!!!          </h1></body></html>          ");
	} else if (htmlflag == 800) {
		Send_UART(
				"<html><head><title>Control Interface</title></head><body><h1>LED 4 is now OFF!!!!          </h1></body></html>          ");
	} else {
		Send_UART(
				"<html><head><title>Control Interface</title></head><body><h1>Welcome to the control server!</h1></body></html>          ");
	}
	while (1) {
		Poll_UART();
		if (Copley_Response == COPLEY_OK)
			break;
	}
	DEBUGOUT("SENT");

	while (waitESP <= 10) {
		Poll_UART();
		if (Copley_Response == SEND_OK) {
			break;
		}
		if (Copley_Response == ESPERROR) {
			return 0;
		}
	}
	Send_UART("AT+CIPCLOSE=0\r\n");
	while (1) {
		Poll_UART();
		if (Copley_Response == COPLEY_OK) {
			break;
		}
		if (Copley_Response == ESPERROR) {
			return 0;
		}
	}
	waitESP = 0;
	while (waitESP <= 10) {
	}
	return 1;

}

void BreakIn() {

	strcpy(checkstring, "1234");
	go = strncmp(checkstring, InBound, 4); // Check this will parse correctly

	if (go == 0) {
		//broken = 1;
		Board_LED_Set(1, true);

		//sscanf(InBound, "%d %d %d %d %d %d %d %d %d", &valid, &counter, &command, &pan
		//	&tilt, &duration, &jpan, &jtilt, &newkp, &newvff);
		token = strtok(InBound, space);
		strcpy(valid, token);
		token = strtok(NULL, space);
		strcpy(char_counter, token);
		counter = atoi(char_counter);

		Board_LED_Set(1, false);

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

			switch (command) {
			case 0:
				DEBUGOUT("DUD\r\n");
			case 1: //Fade

				sprintf(header, "%s", "Moving");

				Fade();

				break;
			case 3:  // Enable

				enablecount++;
				sprintf(header, "%s", "Enable");
				//DEBUGOUT("E, %d\r", enablecount);
				//if (Enable != 1) {
				//Stopped = 0;
				// P = Angle;

				Enable = 1;
				//  led1 = 1;

				//}
				break;
			case 10: //Polling
				if (Enable == 1) {
					sprintf(header, "%s", "Enabled");
				} else {
					sprintf(header, "%s", "Disabled");
				}
				break;
			case 90:  //Stop
				sprintf(header, "%s", "Stopped");
				//if (Enable != 0) {
				Enable = 0;

				//}
				break;
			default:
				DEBUGOUT("GARBAGE\r\n");
			}
		} else {
			//command = -1; //Flag the failure
			// fail++;
			//DEBUGOUT("%d\r", fail);
		}
	}
}

void BuildOut() {
	sendcount++;
	OutBound[0] = '\0';
	ftoa((Drive.sysdojog), panvel, 3);
	ftoa((Drive.SysDemandPos), outpan, 3);
	ftoa((Drive.Fade_Run_Time), outtime, 3);

	//Drivepos =  (Drive.sysdojog * 1000);
	//sprintf(outpan, "%d", (Drivepos));
	sprintf(outsyspos, "%d", 0);
	//sprintf(outtime, "%d", Drivepos);
	sprintf(outspeed, "%d", 0);
	sprintf(outerror, "%d", 0);

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

void sendmetoo(void) {
	while (1) {
		if (printflag) {
			//DEBUGOUT("%s", printme);
			//printme[0] = '\0';
			printflag = false;
		}
		vTaskDelay(configTICK_RATE_HZ / 500);
	}
}

void Copley_Manager(void) {
	while (1) {
		Copley_Configure();
		int running = 1;
		while (running) {
			running = Copley_ConnectionManager();
		}
	}
}

void application(void) {
	//SetupIFTask();
	Set_Loop_Tick_Time(Admin.Loop_Time_ms / 1000);

	while (1) {

		GPIO_Set(34, true);
		if (timer2 >= 25) {
			timer2 = 0;
			//netcheck();
		} else {
			timer2++;
		}

		GPIO_Set(33, toggle);

		P14 = GPIO_Get(14);

		toggle = !toggle;

		if (timer >= 0) {
			timer = 0;
			//tcp_read();
			BreakIn();

		} else {
			timer++;
		}

		AxisProfiler();
		//Servo_Scheduler(&Drive);

		if ((broken >= 1) || (sendtimer >= 500)) {
			//ESP_Connection_Manager();
			//DEBUGOUT("InBound = %s\r\n", InBound);
			sendtimer = 0;
			Board_LED_Set(3, LED_3_toggle);
			LED_3_toggle = !LED_3_toggle;
			broken = 0;
			if (waitESP >= 100) {
				waitESP = 0;
			} else {
				waitESP++;
			}
			//BuildOut();
			//GPIO_Set(34, true);
			//tcp_send(OutBound);

			//Board_LED_Set(3, false);
		}
		sendtimer++;

		GPIO_Set(34, false);

		Admin.Real_Time_ms = Admin.Real_Time_ms + Admin.Loop_Time_ms;

		vTaskDelay(configTICK_RATE_HZ / Admin.TICK_RATE_HZ_div);
	}
}

// ************************************************************************
// Admin data is used to drive the Round Robin
void Admin_setup() {
	Admin.Real_Time_ms = 0;
	Admin.Loop_Time_ms = 1; // Providing a 2Khz Round Robin
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
	Setup_UART_SELECTION();

	//DEBUGOUT("HEEEELLLLOOOO");

	Admin_setup();
	// Initialise the Axis array
	DriveAxisParameters(&Drive, COPLEY);

	//sys_thread_new("sendmetoo", sendmetoo, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);

	xTaskCreate(application, (signed char* ) "Application",
			configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
			(xTaskHandle *) NULL);

	xTaskCreate(Copley_Manager, (signed char* ) "ESP_Manager",
			configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
			(xTaskHandle *) NULL);

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}

/**
 * @}
 */
