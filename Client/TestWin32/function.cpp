#include "stdafx.h"
#include "function.h"
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib,"ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

SOCKET s;
sockaddr_in addr;

int InitSocket() {
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return 0;
	}

	s = socket(
		AF_INET,
		SOCK_STREAM,
		0
	);
	if (s == INVALID_SOCKET) {
		return 0;
	}


	/*
	//这部分可能可以去掉
	sockaddr_in addr;
	int nLength = sizeof(sockaddr_in);

	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(10087);
	//这部分可能可以去掉
	*/


	return 1;
}