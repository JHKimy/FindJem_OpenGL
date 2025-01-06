#include "pch.h"


int main()
{
	// �̷� ������
	std::vector<std::vector<int>> mazeMap;

	// �̷� ������
	std::unique_ptr<MazeGenerator> mazeGenerator;
	mazeGenerator = make_unique<MazeGenerator>(15, 15);

	// �� ���� �� ��ȯ
	mazeGenerator->GeneratePrimMaze();
	mazeGenerator->addEntrances();
	mazeMap = mazeGenerator->GetMaze();

	for (int i = 0; i < mazeMap.size(); ++i) {
		for (int j = 0; j < mazeMap[i].size(); ++j) {
			cout << mazeMap[i][j] << " ";  // ������ �� ���
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

		g_clientSocketes[client_id] = clientSocket;

		cout << "Ŭ���̾�Ʈ ����" << endl;
		cout << client_id << endl;
	}

	SocketUtils::Close(listenSocket);
	SocketUtils::Clear();
}