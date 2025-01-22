#pragma once
#include "pch.h"


class SocketUtils
{
public:
	static void Init();
	static void Clear();
	static SOCKET CreateSocket();
	static bool SetReuseAddress(SOCKET socket, bool flag);
	static bool SetRecvBufferSize(SOCKET socket, int size);
	static bool SetSendBufferSize(SOCKET socket, int size);
	static bool SetTcpNoDelay(SOCKET socket, bool flag);
	static bool Bind(SOCKET socket, SOCKADDR_IN netAddr);
	static bool BindAnyAddress(SOCKET socket, unsigned short port);
	static bool Listen(SOCKET socket, int backlog = SOMAXCONN);
	static void Close(SOCKET& socket);
};

template<typename T>
static inline bool SetSockOpt(SOCKET socket, int level, int optName, T optVal)
{
	return SOCKET_ERROR != ::setsockopt(socket, level, optName, reinterpret_cast<char*>(&optVal), sizeof(T));
}