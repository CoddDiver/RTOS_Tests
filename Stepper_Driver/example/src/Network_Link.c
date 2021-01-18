
#include "lwip/init.h"
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/memp.h"
#include "lwip/tcpip.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/timers.h"
#include "netif/etharp.h"

#include "lwipopts.h"
#include "lwip/sockets.h"

#if LWIP_DHCP
#include "lwip/dhcp.h"
#endif

#include "board.h"
#include "arch\lpc17xx_40xx_emac.h"
#include "arch\lpc_arch.h"
#include "arch\sys_arch.h"
#include "lpc_phy.h"/* For the PHY monitor support */
#include "TCP_Connection.h"
#include "Network_Link.h"


/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* NETIF data */
static struct netif lpc_netif;
int flag = 0;
int send = 0;
char InBound[100];
char OutBound[100];
char stuf2[256];

ip_addr_t ipaddr, netmask, gw;
volatile s32_t tcpipdone = 0;
uint32_t physts;
static int prt_ip = 0;
/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/


/* Callback for TCPIP thread to indicate TCPIP init is done */
void tcpip_init_done_signal(void *arg)
{
	/* Tell main thread TCP/IP init is done */
	*(s32_t *) arg = 1;
}

/* LWIP kickoff and PHY link monitor thread */
//void vSetupIFTask (void *pvParameters) {
void SetupIFTask (void) {


	/* Wait until the TCP/IP thread is finished before
	   continuing or wierd things may happen */
	LWIP_DEBUGF(LWIP_DBG_ON, ("Waiting for TCPIP thread to initialize...\n"));
	tcpip_init(tcpip_init_done_signal, (void *) &tcpipdone);
	while (!tcpipdone) {
		msDelay(1);
	}

	LWIP_DEBUGF(LWIP_DBG_ON, ("Starting LWIP TCP echo server...\n"));

	/* Static IP assignment */
#if LWIP_DHCP
	IP4_ADDR(&gw, 0, 0, 0, 0);
	IP4_ADDR(&ipaddr, 0, 0, 0, 0);
	IP4_ADDR(&netmask, 0, 0, 0, 0);
#else
	IP4_ADDR(&gw, 192, 168, 1, 254);
	IP4_ADDR(&ipaddr, 192, 168, 1, 117);
	IP4_ADDR(&netmask, 255, 255, 255, 0);
#endif

	/* Add netif interface for lpc17xx_8x */
	if (!netif_add(&lpc_netif, &ipaddr, &netmask, &gw, NULL, lpc_enetif_init,
				   tcpip_input)) {
		LWIP_ASSERT("Net interface failed to initialize\r\n", 0);
	}
	netif_set_default(&lpc_netif);
	netif_set_up(&lpc_netif);

	/* Enable MAC interrupts only after LWIP is ready */
	NVIC_SetPriority(ETHERNET_IRQn, config_ETHERNET_INTERRUPT_PRIORITY);
	NVIC_EnableIRQ(ETHERNET_IRQn);

#if LWIP_DHCP
	dhcp_start(&lpc_netif);
#endif

	//socket_fd = socket(PF_INET, SOCK_STREAM, 0);

	/* Initialize and start application */
	tcp_connection_init();
	//int t = 0;
	//while (t != 1){
	//t = tcp_make_connection();
	//}


}


/* This loop monitors the PHY link and will handle cable events
   via the PHY driver. */
void netcheck(void) {
	/* Call the PHY status update state machine once in a while
	   to keep the link status up-to-date */
	physts = lpcPHYStsPoll();

	/* Only check for connection state when the PHY status has changed */
	if (physts & PHY_LINK_CHANGED) {
		if (physts & PHY_LINK_CONNECTED) {
			Board_LED_Set(2, true);
			prt_ip = 0;

			/* Set interface speed and duplex */
			if (physts & PHY_LINK_SPEED100) {
				Chip_ENET_Set100Mbps(LPC_ETHERNET);
				NETIF_INIT_SNMP(&lpc_netif, snmp_ifType_ethernet_csmacd, 100000000);
			}
			else {
				Chip_ENET_Set10Mbps(LPC_ETHERNET);
				NETIF_INIT_SNMP(&lpc_netif, snmp_ifType_ethernet_csmacd, 10000000);
			}
			if (physts & PHY_LINK_FULLDUPLX) {
				Chip_ENET_SetFullDuplex(LPC_ETHERNET);
			}
			else {
				Chip_ENET_SetHalfDuplex(LPC_ETHERNET);
			}

			tcpip_callback_with_block((tcpip_callback_fn) netif_set_link_up,
									  (void *) &lpc_netif, 1);
		}
		else {
			Board_LED_Set(2, false);
			tcpip_callback_with_block((tcpip_callback_fn) netif_set_link_down,
									  (void *) &lpc_netif, 1);
		}

		DEBUGOUT("Link connect status: %d\r\n", ((physts & PHY_LINK_CONNECTED) != 0));

		/* Delay for link detection (250mS) */

	}



	/* Print IP address info */
	if (!prt_ip) {
		if (lpc_netif.ip_addr.addr) {
			static char tmp_buff[16];
			DEBUGOUT("IP_ADDR    : %s\r\n", ipaddr_ntoa_r((const ip_addr_t *) &lpc_netif.ip_addr, tmp_buff, 16));
			DEBUGOUT("NET_MASK   : %s\r\n", ipaddr_ntoa_r((const ip_addr_t *) &lpc_netif.netmask, tmp_buff, 16));
			DEBUGOUT("GATEWAY_IP : %s\r\n", ipaddr_ntoa_r((const ip_addr_t *) &lpc_netif.gw, tmp_buff, 16));
			prt_ip = 1;
		}
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	MilliSecond delay function based on FreeRTOS
 * @param	ms	: Number of milliSeconds to delay
 * @return	Nothing
 * Needed for some functions, do not use prior to FreeRTOS running
 */
void msDelay(uint32_t ms)
{
	vTaskDelay((configTICK_RATE_HZ * ms) / 1000);
}


