#include "pch.h"

//�������� 
vector<SOCKET>	g_clientSocket;
mutex			g_mutex;

int main()
{
	SocketUtils::Init();

	SOCKET listenSocket = SocketUtils::CreateSocket();
	if (listenSocket == INVALID_SOCKET)
		return 0;

	SocketUtils::SetReuseAddress(listenSocket, true);

	if (SocketUtils::BindAnyAddress(listenSocket, SERVER_PORT) == false)
		return 0;

	if (SocketUtils::Listen(listenSocket, SOMAXCONN) == false)
		return 0;

	cout << "���� �����.................." << endl;
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(clientAddr);

	// �� ���� �Լ� ȣ�� 
	
	while (true)
	{
		
	}
	SocketUtils::Close(listenSocket);
	SocketUtils::Clear();
}