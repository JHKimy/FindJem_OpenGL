#include "pch.h"

//전역변수 
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

	cout << "서버 대기중.................." << endl;
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(clientAddr);

	// 맵 생성 함수 호출 
	
	while (true)
	{
		
	}
	SocketUtils::Close(listenSocket);
	SocketUtils::Clear();
}