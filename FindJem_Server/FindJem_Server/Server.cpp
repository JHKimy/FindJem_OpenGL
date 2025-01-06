#include "pch.h"

void Send_Maze_Data(int clientid)
{
	SC_MAZE_INFO p;
	p.packet_size = sizeof(p);
	p.packet_type = SC_MAZE_DATA;

	for (int i{}; i < g_mazeMap.size(); ++i) {
		for (int j{}; j < g_mazeMap[i].size(); ++j) {
			p.mazeMap[i][j] = g_mazeMap[i][j];
		}
	}
		
	if (g_is_accept[clientid]) {
		int retval = send(g_clientSocketes[clientid],
			reinterpret_cast<const char*>(&p), sizeof(p), 0);
		if (retval == SOCKET_ERROR) {
			cout << "fail ! " << clientid << ": " << WSAGetLastError() << endl;
		}
		else {
			cout << "Send to client " << clientid << endl;
		}
	}
}

void HandleThread(int id)
{
	Send_Maze_Data(id);
}


int main()
{

	// 미로 생성기
	std::unique_ptr<MazeGenerator> mazeGenerator;
	mazeGenerator = make_unique<MazeGenerator>(15, 15);

	// 맵 생성 및 변환
	mazeGenerator->GeneratePrimMaze();
	mazeGenerator->addEntrances();
	g_mazeMap = mazeGenerator->GetMaze();

	for (int i = 0; i < g_mazeMap.size(); ++i) {
		for (int j = 0; j < g_mazeMap[i].size(); ++j) {
			cout << g_mazeMap[i][j] << " ";  // 데이터 값 출력
		}
		cout << endl;  // 각 행 끝에 줄 바꿈
	}

	SocketUtils::Init();

	// 리슨 소켓 생성
	SOCKET listenSocket = SocketUtils::CreateSocket();
	if (listenSocket == INVALID_SOCKET)
		return 0;

	// 소켓 옵션
	SocketUtils::SetReuseAddress(listenSocket, true);

	// 주소값 바인딩
	if (SocketUtils::BindAnyAddress(listenSocket, SERVER_PORT) == false)
		return 0;

	// 대기
	if (SocketUtils::Listen(listenSocket, SOMAXCONN) == false)
		return 0;

	cout << "서버 대기중.................." << endl;
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(clientAddr);

	// 맵 생성 함수 호출 
	
	while (true)
	{
		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			return 0;
		}

		int client_id = get_id();
		
		g_is_accept[client_id] = true;

		g_clientSocketes[client_id] = clientSocket;
		
		cout << "클라이언트 접속" << endl;
		cout << client_id << endl;
		g_threads[client_id] = thread{ HandleThread, client_id };
	}
	for (auto& t : g_threads)
		t.join();
	SocketUtils::Close(listenSocket);
	SocketUtils::Clear();
}