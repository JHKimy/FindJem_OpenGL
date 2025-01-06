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

	SocketUtils::Init();

	// ���� ���� ����
	SOCKET listenSocket = SocketUtils::CreateSocket();
	if (listenSocket == INVALID_SOCKET)
		return 0;

	// ���� �ɼ�
	SocketUtils::SetReuseAddress(listenSocket, true);

	// �ּҰ� ���ε�
	if (SocketUtils::BindAnyAddress(listenSocket, SERVER_PORT) == false)
		return 0;

	// ���
	if (SocketUtils::Listen(listenSocket, SOMAXCONN) == false)
		return 0;

	cout << "���� �����.................." << endl;
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(clientAddr);

	// �� ���� �Լ� ȣ�� 
	
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
		
		cout << "Ŭ���̾�Ʈ ����" << endl;
		cout << client_id << endl;
		g_threads[client_id] = thread{ HandleThread, client_id };
	}
	for (auto& t : g_threads)
		t.join();
	SocketUtils::Close(listenSocket);
	SocketUtils::Clear();
}