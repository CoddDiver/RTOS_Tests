/*
 * http_messages.h
 *
 *  Created on: 26 Jun 2020
 *      Author: Andrew
 */

#ifndef SRC_HTTP_MESSAGES_H_
#define SRC_HTTP_MESSAGES_H_

const static char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
char ToSend[800];

void button_selection(const char *name);
void button_selection2(const char *name);
void DoChart(void);

#endif /* SRC_HTTP_MESSAGES_H_ */
