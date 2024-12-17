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

	cout << "���� �����.................." << endl;
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(clientAddr);


	while (true)
	{
		// ������ ó��
	}
	SocketUtils::Close(listenSocket);
	SocketUtils::Clear();
}