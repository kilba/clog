#ifndef NET_H
#define NET_H

#include<winsock2.h>

/* Errors */
enum {
    CLOG_ERR_INVAL_SOCK
};

/* Actions */
enum {
    CLOG_ACT_IGNORE = 1
};

/* Callback Types */
enum {
    CLOG_ONCONNECTION = 0,

    CLOG_CALLBACK_COUNT
};

typedef struct {
	SOCKET sock;

	char *host;
	int port;

	int error;
} Clog;

void net_getStream(Clog *str);

void net_listener(int type, void (*callback)());
int net_send(Clog *stream, char *data, int data_len);
int net_connect(char *host, int port);
int net_listen(char *host, int port);
int net_recv(Clog str, char *msg, int size);
void net_closeStream(Clog *stream);

typedef void(*clog_callback)(Clog stream);

#endif
