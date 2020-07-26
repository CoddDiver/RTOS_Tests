/*
 *
 */
#include "TCP_Connection.h"

#include "lwip/opt.h"
#include <string.h>
#include <stdio.h>

#if LWIP_NETCONN

#include "lwip/sys.h"
#include "lwip/api.h"

extern int flag;
extern int send;
extern char InBound[100];
//extern char OutBound[100];
char ToGo0[100];
char ToGo1[100];
extern char stuf2[256];
struct netconn *conn, *newconn;
err_t err;
int tcpOK = 0;
struct netbuf *buf;
struct buf *in;
void *data0;
u16_t len0 = 100;
void *data1;
u16_t len1;
bool flipflop = false;
bool sendflip = false;
bool sendme = false;
/*-----------------------------------------------------------------------------------*/
//Added to make thread free:


int tcp_make_connection(void){

	 // LWIP_UNUSED_ARG(arg);

	  /* Create a new connection identifier. */
	  conn = netconn_new(NETCONN_TCP);

	  /* Bind connection to well known port number 7. */
	  netconn_bind(conn, NULL, 7);
	  netconn_listen(conn);

	    while (err != ERR_OK) {

	      /* Grab new connection. */
	   err = netconn_accept(conn, &newconn);
	      /*printf("accepted new connection %p\n", newconn);*/
	      /* Process the new connection. */
	    }
	    return 1;
}


static void 
tcp_connection_thread(void *arg)
{


  LWIP_UNUSED_ARG(arg);

  /* Create a new connection identifier. */
  conn = netconn_new(NETCONN_TCP);

  /* Bind connection to well known port number 7. */
  netconn_bind(conn, NULL, 7);

  /* Tell connection to go into listening mode. */
  netconn_listen(conn);

  while (1) {

    /* Grab new connection. */
    err = netconn_accept(conn, &newconn);
    /*printf("accepted new connection %p\n", newconn);*/
    /* Process the new connection. */
    if (err == ERR_OK) {
    	tcpOK = 1;
      while ((err = netconn_recv(newconn, &buf)) == ERR_OK) {
        /*printf("Recved\n");*/
        do {
           if (flipflop){
        	netbuf_data(buf, &data0, &len0);
           } else {
        	netbuf_data(buf, &data1, &len1);
           }
             //err = netconn_write(newconn, &OutBound, sizeof(OutBound), NETCONN_COPY);

#if 1
            if (err != ERR_OK) {
            	DEBUGOUT("tcp \"%s\"\n", lwip_strerr(err));
            }
#endif
        } while (netbuf_next(buf) >= 0);
        netbuf_delete(buf);
        flipflop = !flipflop;

        if (sendme){
        	sendme = false;
        	if (sendflip){
        		netconn_write(newconn, &ToGo1, sizeof(ToGo0), NETCONN_COPY);
        		sendflip = false;
        	} else {
        		netconn_write(newconn, &ToGo0, sizeof(ToGo1), NETCONN_COPY);
        		sendflip = true;
        	}
        	sendme = false;
        }

      }

      /*printf("Got EOF, looping\n");*/ 
      /* Close connection and discard connection identifier. */
      netconn_close(newconn);
      netconn_delete(newconn);
      tcpOK = 0;
    }
  }
}
/*-----------------------------------------------------------------------------------*/
void tcp_read(void){

	if (flipflop){
      	strncpy (InBound, data1, 99); // When flipflop high it buffers into data0
      	InBound[99] = '\0';
    } else {
       	strncpy (InBound, data0, 99); // When flipflop high it buffers into data1
       	InBound[99] = '\0';
    }

}
void tcp_send(char _packet[]){

	if (sendflip){
		strcpy(ToGo0, _packet);
	} else {
		strcpy(ToGo1, _packet);
	}
    sendme = 1;
	//netconn_write(newconn, &ToGo, sizeof(ToGo), NETCONN_COPY);

}

void tcp_connection_init(void)
{
  // tcp_make_connection();
  sys_thread_new("tcp_connection_thread", tcp_connection_thread, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);

}

/*-----------------------------------------------------------------------------------*/

#endif /* LWIP_NETCONN */
