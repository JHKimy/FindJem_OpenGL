#include "pch.h"

int main()
{
	SocketUtils::Init();

	SOCKET listenSocket = SocketUtils::CreateSocket();
	if (listenSocket == INVALID_SOCKET)
		return 0;

	SocketUtils::SetReuseAddress(listenSocket, true);

	if (SocketUtils::BindAnyAddress(listenSocket, 9000) == false)
		return 0;

	if (SocketUtils::Listen(listenSocket, SOMAXCONN) == false)
		return 0;

	cout << "서버 대기중.................." << endl;
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(clientAddr);


	while (true)
	{
		// 스레드 처리
	}
	SocketUtils::Close(listenSocket);
	SocketUtils::Clear();
}