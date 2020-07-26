/*
 * http_messages.h
 *
 *  Created on: 26 Jun 2020
 *      Author: Andrew
 */

#ifndef SRC_HTTP_MESSAGES_H_
#define SRC_HTTP_MESSAGES_H_

static char http_full_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n<html><head><title>Congrats!</title></head><body><h1>Welcome to the control server!</h1><p>This is a small test page, served by httpserver-netconn.</body></html></    ";

static char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n"; //46
//162
static char http_index_html[] = "<html><head><title>Congrats!</title></head><body><h1>Welcome to the control server!</h1><p>This is a small test page, served by httpserver-netconn.</body></html></";
char ToSend[800];

void button_selection(const char *name);
void button_selection2(const char *name);
void DoChart(void);

#endif /* SRC_HTTP_MESSAGES_H_ */
