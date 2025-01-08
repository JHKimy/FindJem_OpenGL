#include "pch.h"

void Send_Maze_Data(int clientid)
{
	// 미로 정보 담는 패킷 구조체
	SC_MAZE_INFO p;
	p.packet_size = sizeof(p);
	p.packet_type = SC_MAZE_DATA;
	p.player_id = clientid;
	p.x = g_characters[clientid].GetPostionX();
	p.y = g_characters[clientid].GetPostionY();
	p.z = g_characters[clientid].GetPostionZ();
	// 미로 데이터 패킷 구조체에 복사
	for (int i{}; i < g_mazeMap.size(); ++i) {
		for (int j{}; j < g_mazeMap[i].size(); ++j) {
			p.mazeMap[i][j] = g_mazeMap[i][j];
		}
	}
		
	
	if (g_is_accept[clientid]) {
		// 데이터를 클라이언트 소켓으로 전송
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

void Send_My_Character_Data(int clientid)
{
	// 미로 정보 담는 패킷 구조체
	SC_CHARACTER_MOVE_PACKET p;
	p.packet_size = sizeof(p);
	p.packet_type = SC_CHARACTER_MOVE;
	p.player_id = clientid;
	p.PosX = g_characters[clientid].GetPostionX();
	p.PosY = g_characters[clientid].GetPostionY();
	p.PosZ = g_characters[clientid].GetPostionZ();

	p.DirX = g_characters[clientid].GetForwordVecX();
	p.DirZ = g_characters[clientid].GetForwordVecZ();

	if (g_is_accept[clientid]) {
		// 데이터를 클라이언트 소켓으로 전송
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
	Character character{ id };
	{
		lock_guard<mutex> lock(g_character_mutex);
		g_characters[id] = character;
		g_characters[id].playerID = id;
	}
	Send_Maze_Data(id);
	while (true)
	{
		char buf[1024];
		// 패킷 전체 데이터 수신
		int retval = recv(g_clientSocketes[id], buf, sizeof(buf), 0);

		if (retval <= 0) {
			// 연결 종료 또는 오류 처리
			if (retval == 0) {
				std::cout << "Client disconnected." << std::endl;
			}
			else {
				std::cout << "Recv error: " << WSAGetLastError() << std::endl;
			}
		}

		char packetType = static_cast<char>(buf[1]);

		// 패킷 처리
		switch (packetType) {
		case CS_READY: {
			CS_READY_PACKET* p = reinterpret_cast<CS_READY_PACKET*>(buf);
			g_characters[id].isReady = true;

			for (auto& other : g_characters) {
				if (other.playerID == p->player_id) continue;
				{
					lock_guard<mutex> lock(g_character_mutex);
					if (!g_is_accept[other.playerID]) continue;
				}
				SC_ADD_CHARACTER_PACKET packet;
				packet.player_id = id;

				packet.packet_size = sizeof(packet);
				packet.packet_type = SC_ADD_CHARACTER;
				packet.PosX = g_characters[id].GetPostionX();
				packet.PosY = g_characters[id].GetPostionY();
				packet.PosZ = g_characters[id].GetPostionZ();
				int retval = send(g_clientSocketes[other.playerID],
					reinterpret_cast<const char*>(&packet), sizeof(packet), 0);
				cout << "누구에게 감 ? : " << other.playerID << endl;
				cout << "posx : " << packet.PosX << endl;
				cout << "posy : " << packet.PosY << endl;
				cout << "id : " << packet.player_id << endl;
		
			}
			break;
		}
		case CS_PLAYER: {
			CS_PLAYER_PACKET* p = reinterpret_cast<CS_PLAYER_PACKET*>(buf);
			g_characters[p->player_id].Move(p->direction);
			g_characters[p->player_id].Rotate(p->dirY);
			Send_My_Character_Data(p->player_id);

			//for (auto& other : g_characters)
			//{

			//}
			break;
		}default:
			std::cout << "Unknown packet type: " << packetType << std::endl;
			break;
		}
	}
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

	// 1. 소켓 초기화 
	SocketUtils::Init();

	// 2. 리슨 소켓 생성
	SOCKET listenSocket = SocketUtils::CreateSocket();
	if (listenSocket == INVALID_SOCKET)
		return 0;

	// 3. 소켓 옵션
	SocketUtils::SetReuseAddress(listenSocket, true);

	// 4. 주소값 바인딩
	if (SocketUtils::BindAnyAddress(listenSocket, SERVER_PORT) == false)
		return 0;

	// 5. 대기
	if (SocketUtils::Listen(listenSocket, SOMAXCONN) == false)
		return 0;

	cout << "서버 대기중.................." << endl;
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(clientAddr);

	
	// 6. 클라이언트 접속 기다리는 루프
	while (true)
	{
		// 7. 클라이언트 접속 요청 수락
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

		// 8. 새로운 스레드 생성하여 클라이언트 요청 처리
		g_threads[client_id] = thread{ HandleThread, client_id };
	}

	// 9. 모든 스레드 종료될때 까지 기다림
	for (auto& t : g_threads)
		t.join();

	// 10. 리소스 정리
	SocketUtils::Close(listenSocket);
	SocketUtils::Clear();
}