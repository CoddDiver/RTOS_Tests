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
#define REQ_LINE_BUFF_SIZE 100
#define UNKNOWN 0
#define COPLEY_OK 1
#define ESPERROR 2
#define FAIL 3
#define READY 4
#define READY_TO_WRITE_TCP 5
#define SEND_OK 6
#define ZERO_CONNECT 7
#define ONE_CONNECT 8
#define TWO_CONNECT 9
#define THREE_CONNECT 10
#define ZERO_CLOSED 11
#define ONE_CLOSED 12
#define TWO_CLOSED 13
#define THREE_CLOSED 14

/* Transmit and receive ring buffer sizes */
#define UART_SRB_SIZE 256	/* Send */
#define UART_RRB_SIZE 256	/* Receive */

void HANDLER_NAME(void);
void Send_UART(char* ToSend);
void Poll_UART(void);
void Poll_UART2(void);
void processHTML(void);
void Setup_UART_SELECTION(void);
void deinnit_UART_SELECTION(void);
int readCopleyResponse();


#endif /* SRC_UART_MANAGER_H_ */
