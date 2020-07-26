/*
*
*/

#include <stdio.h>
#include <string.h>
#//include "lwip/opt.h"
//#include "lwip/arch.h"
//#include "lwip/api.h"
//#include "lwip_fs.h"
#include "http_messages.h"
#include "board.h"
#include "UART_Manager.h"

int httpdata = 1234;
struct netbuf *buf;
struct buf *in;
bool served = false;
void *data0;

void *data1;

char InBound[256];
char OutBound[300];
bool toggle3 = true;
bool toggle4 = true;
extern char NEMA[256];

//const static char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";

/* Dynamic header generation based on Filename */
void http_server_netconn_init(void);

#ifndef CRLF
#define CRLF "\r\n"
#endif

/* Function to check if the requested method is supported */
static int supported_method(const char *method)
{
	if (strncmp(method, "GET", 3) == 0)
		return 1;
	if (strncmp(method, "POST", 4) == 0)
		return 1;
	return 0;
}

/* Function to extract version information from URI */
static uint32_t get_version(const char *vstr)
{
	int major = 0, minor = 0;
	sscanf(vstr, "HTTP/%d.%d", &major, &minor);
	return (major << 16) | minor;
}

/** Serve one HTTP connection accepted in the http thread */
void
http_server_netconn_serve2(void)
{

  char buf[256];
  char *tbuf;
  buf[0]='\0';
	int len;
	uint16_t buflen = 0;
	strncpy(InBound, NEMA, 50);
	//strncpy(buf, NEMA, 50);

	DEBUGOUT("InBound %s \r\n", InBound);
	if (buflen < 5 || strstr(InBound, CRLF) == NULL) {
		//DEBUGOUT("InBound %s \r\n", InBound);
		//DEBUGOUT("HTTPD: Invalid Request \n\r");
		//LWIP_DEBUGF(HTTPD_DEBUG, ("HTTPD: Invalid Request Line\r\n"));
		goto close_and_exit;
	}

	tbuf = strchr(InBound, ' ');
	if (tbuf == NULL) {
		DEBUGOUT("Method \n\r");
		//LWIP_DEBUGF(HTTPD_DEBUG, ("HTTPD: Parse error in Request Line\r\n"));
		goto close_and_exit;
	}
	
	*tbuf++ = 0;

	//buf = tbuf;
	tbuf = strchr(InBound, ' ');
	if (tbuf == NULL) {
		//DEBUGOUT("Version String Not found \n\r");
		//LWIP_DEBUGF(HTTPD_DEBUG, ("HTTPD: Version string not found: %s\r\n", buf));
	} else {
		*tbuf++ = 0;
		//req_ver = get_version(tbuf);
		//DEBUGOUT("HTTPD: Req Version \n\r");
		//LWIP_DEBUGF(HTTPD_DEBUG | LWIP_DBG_TRACE, ("HTTPD: Request version %d.%d\r\n",
		//	req_ver >> 16, req_ver & 0xFFFF));
	}
	
	tbuf = strchr(InBound, '?');
	if (tbuf != NULL) {
		//DEBUGOUT("HTTPD: Arguments \n\r");
		//LWIP_DEBUGF(HTTPD_DEBUG, ("HTTPD: Arguments %s in URI ignored\r\n", tbuf));
		*tbuf++ = 0;
	}
	tbuf = strstr(InBound, "Buttons");
	if (tbuf != NULL) {
		DEBUGOUT("Serving Buttons \r\n");

		if (strstr(InBound, "Buttons_1")!=NULL){ Board_LED_Set(3, toggle3); toggle3 = !toggle3;}
		if (strstr(InBound, "Buttons_2")!=NULL){ Board_LED_Set(4, toggle4); toggle4 = !toggle4;}

	     button_selection2("Done");
	      //DoChart();
	     DEBUGOUT("%s \r", ToSend);

	     //netconn_write(conn, ToSend, sizeof(ToSend)-1, NETCONN_NOCOPY);

	     DEBUGOUT("Gone Buttons \r");
		goto close_and_exit;

	}
	if (1) {
		DEBUGOUT("Serving %s \r\n", buf);

		SendCMD(http_html_hdr);
			/* No home page, send if from buffer */
     // netconn_write(conn, http_html_hdr, sizeof(http_html_hdr)-1, NETCONN_NOCOPY);
     // netconn_write(conn, http_form_html, sizeof(http_form_html)-1, NETCONN_NOCOPY);
     // button_selection("Welcome");
     // button_selection2("LED 1");
     // netconn_write(conn, ToSend, sizeof(ToSend)-1, NETCONN_NOCOPY);
     // netconn_write(conn, http_index_html, sizeof(http_index_html)-1, NETCONN_NOCOPY);
    //  http_index_html
      //err = netconn_recv(conn, &inbuf);
      //netbuf_data(inbuf, (void**)&buf, &buflen);

      	DEBUGOUT("Gone \r");
			goto close_and_exit;

	}
	
close_and_exit:{}


}



/*********************************************************************//**
 * @brief	Blinky Initialization function
 *
 * This function is called by the common dual-core init,
 * which will start the HTTP server and will serve requests received.
 *
 * @return	None
 **********************************************************************/
void
http_server_netconn_init(void)
{
 // sys_thread_new("http_server_netconn", http_server_netconn_thread, NULL, DEFAULT_THREAD_STACKSIZE + 128, DEFAULT_THREAD_PRIO);
}


