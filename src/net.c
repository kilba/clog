#include <winsock2.h>
#include <windows.h>

#include <net.h>

void clog_emptyCallback(Clog stream) { };

clog_callback callbacks[CLOG_CALLBACK_COUNT];

void clog_listener(int type, clog_callback callback) {
    if(callback == NULL)
        return;

    callbacks[type] = callback;
}

int clog_send(Clog *stream, char *data, int data_len) {
    int success = send(stream->sock, data, data_len, 0);
    if(success < 0)
        return WSAGetLastError();

    return success;
}

int clog_recv(Clog str, char *msg, int size) {
    int num_bytes = recv(str.sock, msg, size, 0);
    if(num_bytes == SOCKET_ERROR)
        return WSAGetLastError();

    return num_bytes;
}

int clog_connect(char* host, int port) {
    WSADATA wsa;
    Clog this;

    // Socket Init
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return WSAGetLastError();

    if((this.sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        return WSAGetLastError();

    // Socket Init
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(host);
    server.sin_family      = AF_INET;
    server.sin_port        = htons(port);

    // Connect
    if (connect(this.sock, (struct sockaddr*)&server , sizeof(server)) < 0)
        return WSAGetLastError();

    return 0;
}

DWORD WINAPI clog_wait(void* vargp) {
    while(1) {
        struct sockaddr_in client;
        SOCKET c_sock;
        SOCKET sock = *(SOCKET*)vargp;

        int c = sizeof(struct sockaddr_in);
        c_sock = accept(sock, (struct sockaddr*)&client, &c);

        Clog stream;
        stream.sock = c_sock;
        stream.host = inet_ntoa(client.sin_addr);
        stream.port = ntohs(client.sin_port);

	if(c_sock == INVALID_SOCKET)
	    stream.error = CLOG_ERR_INVAL_SOCK;

        callbacks[CLOG_ONCONNECTION](stream);
    }

    return 0;
}

int clog_listen(char *host, int port) {
    WSADATA wsa;
    Clog this;
    
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return WSAGetLastError();

    if((this.sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        return WSAGetLastError();

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(host);
    server.sin_family      = AF_INET;
    server.sin_port        = htons(port);

    if(bind(this.sock,(struct sockaddr*)&server , sizeof(server)) == SOCKET_ERROR)
	return WSAGetLastError();

    listen(this.sock, SOMAXCONN);
    CreateThread(NULL, 0, clog_wait, &this.sock, 0, NULL);

    // closesocket(sock);
	// WSACleanup();

    return 0;
}

void clog_closeStream(Clog *stream) {
    closesocket(stream->sock);
}

void clog_exit() {
}
