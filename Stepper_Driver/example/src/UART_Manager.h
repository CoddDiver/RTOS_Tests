/*
 * UART_Manager.h
 *
 *  Created on: 1 Jul 2020
 *      Author: Andrew
 */

#ifndef SRC_UART_MANAGER_H_
#define SRC_UART_MANAGER_H_

#define UART_SELECTION 	LPC_UART4
#define IRQ_SELECTION 	UART4_IRQn
#define HANDLER_NAME 	UART4_IRQHandler

#define BUFFER_SIZE 256
//#define REQ_LINE_BUFF_SIZE 100



/* Transmit and receive ring buffer sizes */
#define UART_SRB_SIZE 256	/* Send */
#define UART_RRB_SIZE 256	/* Receive */

void HANDLER_NAME(void);
void Send_UART(char* ToSend);
void  Poll_UART_For_String(char *_response);
void Poll_UART(void);
void Setup_UART_SELECTION(int _baud);
void deinnit_UART_SELECTION(void);
//int readCopleyResponse();
int pollCopley();


#endif /* SRC_UART_MANAGER_H_ */
