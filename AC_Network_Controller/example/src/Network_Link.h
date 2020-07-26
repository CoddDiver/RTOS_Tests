/*
 * Network_Link.h
 *
 *  Created on: 10 Jun 2020
 *      Author: Andrew
 */

#ifndef SRC_NETWORK_LINK_H_
#define SRC_NETWORK_LINK_H_

//void tcpip_init_done_signal(void *arg);
void vSetupIFTask (void *pvParameters);
void SetupIFTask (void);
void netcheck(void);
//void msDelay(uint32_t ms);


#endif /* SRC_NETWORK_LINK_H_ */
