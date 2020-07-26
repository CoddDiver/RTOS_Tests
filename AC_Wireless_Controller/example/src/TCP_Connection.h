/*
 *
 */

#ifndef __TCP_CONNECTION_H__
#define __TCP_CONNECTION_H__

void tcp_connection_init(void);
int tcp_make_connection(void);
void tcp_read(void);
void tcp_send(char _packet[]);

#endif /* __TCP_CONNECTION_H__ */
