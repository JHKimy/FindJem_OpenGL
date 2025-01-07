#include "pch.h"

void Send_Maze_Data(int clientid)
{
	// �̷� ���� ��� ��Ŷ ����ü
	SC_MAZE_INFO p;
	p.packet_size = sizeof(p);
	p.packet_type = SC_MAZE_DATA;
	p.player_id = clientid;
	// �̷� ������ ��Ŷ ����ü�� ����
	for (int i{}; i < g_mazeMap.size(); ++i) {
		for (int j{}; j < g_mazeMap[i].size(); ++j) {
			p.mazeMap[i][j] = g_mazeMap[i][j];
		}
	}
		
	
	if (g_is_accept[clientid]) {
		// �����͸� Ŭ���̾�Ʈ �������� ����
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
	// �̷� ������
	std::unique_ptr<MazeGenerator> mazeGenerator;
	mazeGenerator = make_unique<MazeGenerator>(15, 15);

	// �� ���� �� ��ȯ
	mazeGenerator->GeneratePrimMaze();
	mazeGenerator->addEntrances();
	g_mazeMap = mazeGenerator->GetMaze();

	for (int i = 0; i < g_mazeMap.size(); ++i) {
		for (int j = 0; j < g_mazeMap[i].size(); ++j) {
			cout << g_mazeMap[i][j] << " ";  // ������ �� ���
		}
		cout << endl;  // �� �� ���� �� �ٲ�
	}

	// 1. ���� �ʱ�ȭ 
	SocketUtils::Init();

	// 2. ���� ���� ����
	SOCKET listenSocket = SocketUtils::CreateSocket();
	if (listenSocket == INVALID_SOCKET)
		return 0;

	// 3. ���� �ɼ�
	SocketUtils::SetReuseAddress(listenSocket, true);

	// 4. �ּҰ� ���ε�
	if (SocketUtils::BindAnyAddress(listenSocket, SERVER_PORT) == false)
		return 0;

	// 5. ���
	if (SocketUtils::Listen(listenSocket, SOMAXCONN) == false)
		return 0;

	cout << "���� �����.................." << endl;
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(clientAddr);

	
	// 6. Ŭ���̾�Ʈ ���� ��ٸ��� ����
	while (true)
	{
		// 7. Ŭ���̾�Ʈ ���� ��û ����
		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			return 0;
		}

		int client_id = get_id();
		
		g_is_accept[client_id] = true;

		g_clientSocketes[client_id] = clientSocket;
		
		cout << "Ŭ���̾�Ʈ ����" << endl;
		cout << client_id << endl;

		// 8. ���ο� ������ �����Ͽ� Ŭ���̾�Ʈ ��û ó��
		g_threads[client_id] = thread{ HandleThread, client_id };
	}

	// 9. ��� ������ ����ɶ� ���� ��ٸ�
	for (auto& t : g_threads)
		t.join();

	// 10. ���ҽ� ����
	SocketUtils::Close(listenSocket);
	SocketUtils::Clear();
}