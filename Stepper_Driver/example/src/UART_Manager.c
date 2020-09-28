/*
 * UART_Manager.c
 *
 *  Created on: 1 Jul 2020
 *      Author: Andrew
 */
#include "UART_Manager.h"
#include "board.h"
#include "chip.h"
#include <stdio.h>
#include <string.h>

/* Transmit and receive ring buffers */
STATIC RINGBUFF_T txring, rxring;

/* Transmit and receive buffers */
static uint8_t rxbuff[UART_RRB_SIZE], txbuff[UART_SRB_SIZE];

char READ[50];
char NEMA[50];
int ln = 0;
int ld = 0;
int j = 0;

uint8_t key;
char longkey[10];
int bytes;

void HANDLER_NAME(void) {
	Chip_UART_IRQRBHandler(UART_SELECTION, &rxring, &txring);
}

void Setup_UART_SELECTION(int _baud) {
//	Board_UART_Init(UART_SELECTION);

	/* Setup UART for 115.2K8N1 */
	Chip_UART_Init(UART_SELECTION);
	Chip_UART_SetBaud(UART_SELECTION, _baud);
	Chip_UART_ConfigData(UART_SELECTION, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_SetupFIFOS(UART_SELECTION,
			(UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
	Chip_UART_TXEnable(UART_SELECTION);

	/* Before using the ring buffers, initialize them using the ring
	 buffer init function */
	RingBuffer_Init(&rxring, rxbuff, 1, UART_RRB_SIZE);
	RingBuffer_Init(&txring, txbuff, 1, UART_SRB_SIZE);

	/* Reset and enable FIFOs, FIFO trigger level 3 (14 chars) */
	Chip_UART_SetupFIFOS(UART_SELECTION, (UART_FCR_FIFO_EN | UART_FCR_RX_RS |
	UART_FCR_TX_RS | UART_FCR_TRG_LEV3));

	/* Enable receive data and line status interrupt */
	Chip_UART_IntEnable(UART_SELECTION, (UART_IER_RBRINT | UART_IER_RLSINT));

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(IRQ_SELECTION, 1);
	NVIC_EnableIRQ(IRQ_SELECTION);
}

void Send_UART(char *ToSend) {
	Chip_UART_SendRB(UART_SELECTION, &txring, ToSend, strlen(ToSend));
}

void Poll_UART_For_String(char *_response) {
	bytes = Chip_UART_ReadRB(UART_SELECTION, &rxring, &longkey, 10);
	if (bytes > 0){
		strcpy(_response, longkey);
	} else {
		strcpy(_response, "  ");
	}

}

void Poll_UART_char(void) {
	bytes = Chip_UART_ReadRB(UART_SELECTION, &rxring, &key, 1);
	if (bytes > 0) {
		NEMA[j] = key;
		//DEBUGOUT("%c", key);
		if (NEMA[j] == '\r') {
			NEMA[j + 1] = '\n';
			NEMA[j + 2] = '\0';
			ln = j + 2;
			DEBUGOUT("out = %s", NEMA);
			j = 0;
		} else {
			j++;
		}
	}
}

void deinnit_UART_SELECTION(void) {
	/* DeInitialize UART0 peripheral */
	NVIC_DisableIRQ(IRQ_SELECTION);
	Chip_UART_DeInit(UART_SELECTION);
}


