#include "pch.h"


int main()
{
	// 미로 데이터
	std::vector<std::vector<int>> mazeMap;

	// 미로 생성기
	std::unique_ptr<MazeGenerator> mazeGenerator;
	mazeGenerator = make_unique<MazeGenerator>(15, 15);

	// 맵 생성 및 변환
	mazeGenerator->GeneratePrimMaze();
	mazeGenerator->addEntrances();
	mazeMap = mazeGenerator->GetMaze();

	for (int i = 0; i < mazeMap.size(); ++i) {
		for (int j = 0; j < mazeMap[i].size(); ++j) {
			cout << mazeMap[i][j] << " ";  // 데이터 값 출력
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

		g_clientSocketes[client_id] = clientSocket;

		cout << "클라이언트 접속" << endl;
		cout << client_id << endl;
	}

	SocketUtils::Close(listenSocket);
	SocketUtils::Clear();
}