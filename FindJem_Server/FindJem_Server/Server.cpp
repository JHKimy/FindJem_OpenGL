#include "pch.h"
#include "AstarPath.h"

void HandleBulletEnemyCollisions()
{
	// ��� ĳ���Ϳ� ���ؼ�
	for (int i{}; i < g_characters.size(); ++i) {
		for (auto& enemy : g_enemies)
		{
			if (!enemy->IsActive()) continue;


			for (auto& bullet : g_characters[i].bulletPool.pool)
			{
				if (!bullet->IsActive()) continue;

				float distance = V::Distance(bullet->GetPosition(), enemy->GetPosition());

				if (distance < (bullet->boundingRadius + enemy->boundingRadius))
				{
					vec3 bulletDirection = V::Normalize(bullet->GetDirection());
					enemy->TakeDamage(1, bulletDirection); // �� ü�� ����
					bullet->DeActivate();                 // �Ѿ� ��Ȱ��ȭ

					printf("Enemy hit\n");

					if (!enemy->IsActive())
					{
						printf("Enemy defeated\n");
						g_characters[i].IncrementDefeatedEnemies(); // �� ���� ī��Ʈ ����
						break;
					}
				}
			}
		}
	}
}




void Send_Maze_Data(int clientid)
{
	// �̷� ���� ��� ��Ŷ ����ü
	SC_MAZE_INFO p;
	p.packet_size = sizeof(p);
	p.packet_type = SC_MAZE_DATA;
	p.player_id = clientid;

	p.x = g_characters[clientid].GetPostionX();
	p.y = g_characters[clientid].GetPostionY();
	p.z = g_characters[clientid].GetPostionZ();

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

void Send_My_Character_Data(int clientid)
{
	// �̷� ���� ��� ��Ŷ ����ü
	SC_CHARACTER_MOVE_PACKET p;
	p.packet_size = sizeof(p);
	p.packet_type = SC_CHARACTER_MOVE;
	p.player_id = clientid;
	p.PosX = g_characters[clientid].GetPostionX();
	p.PosY = g_characters[clientid].GetPostionY();
	p.PosZ = g_characters[clientid].GetPostionZ();
	p.yaw = g_characters[clientid].GetYaw();

	p.DirX = g_characters[clientid].GetForwordVecX();
	p.DirZ = g_characters[clientid].GetForwordVecZ();

	if (g_is_accept[clientid]) {
		// �����͸� Ŭ���̾�Ʈ �������� ����
		int retval = send(g_clientSocketes[clientid],
			reinterpret_cast<const char*>(&p), sizeof(p), 0);

	}
}

void Send_Enemy_Data(int clientid)
{
	// �� ���� ��� ��Ŷ ����ü
	SC_ENEMY_PACKET p;
	p.packet_size = sizeof(p);
	p.packet_type = SC_ENEMY;


	p.PosX = g_enemies[0]->GetPostionX();
	p.PosY = g_enemies[0]->GetPostionY();
	p.PosZ = g_enemies[0]->GetPostionZ();
	p.enemy_id = g_enemies[0]->GetEnemyID();




	if (g_is_accept[clientid]) {
		cout << " send enemy to clientid : " << clientid << endl;
		// �����͸� Ŭ���̾�Ʈ �������� ����
		int retval = send(g_clientSocketes[clientid],
			reinterpret_cast<const char*>(&p), sizeof(p), 0);

	}
}


void Send_Bullet_Data(int clientid)
{

	
	auto& bullets = g_characters[clientid].bulletPool.pool;

	for (int i{}; i< bullets.size();++i)
	{
		if (bullets[i]->IsActive()) 
		{
			// �� ���� ��� ��Ŷ ����ü
			SC_BULLET_PACKET p;

			p.packet_size = sizeof(p);
			p.packet_type = SC_BULLET;
			p.bullet_id = i;
			p.PosX = bullets[i]->GetPosition().x;
			p.PosY = bullets[i]->GetPosition().y;
			p.PosZ = bullets[i]->GetPosition().z;
		
			if (g_is_accept[clientid]) {
				cout << " send enemy to clientid : " << clientid << endl;
				// �����͸� Ŭ���̾�Ʈ �������� ����
				int retval = send(g_clientSocketes[clientid],
					reinterpret_cast<const char*>(&p), sizeof(p), 0);

			}
		}
	}
	
	
	// for()

	//p.PosX = g_enemies[0]->GetPostionX();
	//p.PosY = g_enemies[0]->GetPostionY();
	//p.PosZ = g_enemies[0]->GetPostionZ();
	//p.enemy_id = g_enemies[0]->GetEnemyID();




}


void EnemyThread()
{

	// �̷� ���� ��� ��Ŷ ����ü
	SC_ENEMY_PACKET p;
	p.packet_size = sizeof(p);
	p.packet_type = SC_ENEMY;


	while (true) {
		std::lock_guard<std::mutex> lock(g_EnemyMutex);
		if (g_characters[0].isReady){
			for (int i{}; i < g_characters.size(); ++i){
				for (int j{}; j < g_enemies.size(); ++j){
					
					if (g_is_accept[i]) {
						// �÷��̾�� �� �Ÿ�
						float distanceToPlayer = g_enemies[j]->DistanceToPlayer(j, i);
						// ���¿� ���� �ൿ ����

						//std::cout << " current : " << (int)g_enemies[j]->currentState << endl;

						switch (g_enemies[j]->currentState)
						{
						case EnemyState::Patrol:
							// Ž�� �������� �Ÿ��� ���
							if (g_enemies[j]->detectionRadius > distanceToPlayer){
								g_enemies[j]->currentState = EnemyState::Chase;
							}
							else {
								g_enemies[j]->Patrol(g_mazeMap);
							}
							break;

						case EnemyState::Chase:
							
							if (distanceToPlayer > g_enemies[j]->chaseRadius) {
								g_enemies[j]->currentState = EnemyState::Patrol;
							}
							else {
								if (!g_enemies[j]->detectPath)
								{
									g_enemies[j]->MakeAStarPath(j, i);
									// ��� ���� �Ϸ�
									g_enemies[j]->detectPath = true;
								}
								g_enemies[j]->Chase();
							}
							break;
						}

						//===================================
						p.enemy_id = g_enemies[j]->GetEnemyID();
						p.bActive = g_enemies[j]->IsActive();
						p.PosX = g_enemies[j]->GetPostionX();
						p.PosY = g_enemies[j]->GetPostionY();
						p.PosZ = g_enemies[j]->GetPostionZ();

					}
				}




				if (g_is_accept[i]) {
					// �����͸� Ŭ���̾�Ʈ �������� ����
					int retval = send(g_clientSocketes[i],
						reinterpret_cast<const char*>(&p), sizeof(p), 0);
					if (retval == SOCKET_ERROR) {
						cout << "fail ! " << i << ": " << WSAGetLastError() << endl;
					}
					else {
						//cout << "Send to client " << i << endl;
					}

				}

			}

			std::this_thread::sleep_for(std::chrono::milliseconds(100)); // CPU ������ ����
		}

	}
}

void BulletTread()
{
	// �������� Ŭ��� ������ ��Ŷ
	SC_BULLET_PACKET p;
	p.packet_size = sizeof(p);
	p.packet_type = SC_BULLET;

	while (true) {
		//std::this_thread::sleep_for(std::chrono::milliseconds(500)); // CPU ������ ����

		std::lock_guard<std::mutex> lock(g_BulletMutex); // Mutex�� ��ȣ
		if (g_characters[0].isReady) {
			for (int i{}; i < g_characters.size(); ++i)
			{
				for (int j{};j<g_characters[i].bulletPool.pool.size();++j)
				{
					if (g_is_accept[i]) 
					{
						if (g_characters[i].bulletPool.pool[j]->IsActive()) 
						{
							//g_characters[i].bulletPool.pool[j]->Update();

							g_characters[i].bulletPool.UpdateAllBullets
							(vec3(g_characters[i].GetPostionX(), g_characters[i].GetPostionY(), g_characters[i].GetPostionZ()));


							
							p.player_id = i;
							p.bullet_id = j;
							p.PosX = g_characters[i].bulletPool.pool[j]->GetPosition().x;
							p.PosY = g_characters[i].bulletPool.pool[j]->GetPosition().y + 1.f;
							p.PosZ = g_characters[i].bulletPool.pool[j]->GetPosition().z;
							p.bActive = g_characters[i].bulletPool.pool[j]->IsActive();


							HandleBulletEnemyCollisions();

							std::this_thread::sleep_for(std::chrono::microseconds(1)); // 10ms ���

							//std::lock_guard<std::mutex> lock(g_BulletMutex);
							//cout << "id : " << p.player_id << endl;
							//cout << "bullet : " << p.PosX << " , " << p.PosZ << endl;

							int retval = send(g_clientSocketes[i],
								reinterpret_cast<const char*>(&p), sizeof(p), 0);
						}
					}
				}
			}
		}

	}
}






// Ŭ�� ��û ó���ϴ� ���� ������ �Լ�
void HandleThread(int id)
{
	// ���� ĳ���� ����
	Character character{ id };
	{
		lock_guard<mutex> lock(g_character_mutex);
		g_characters[id] = character;
		g_characters[id].playerID = id;
	}

	// �̷� ������ ������
	Send_Maze_Data(id);


	while (true)
	{
		char buf[BUFSIZE];

		// ��Ŷ ��ü ������ ����
		int retval = recv(g_clientSocketes[id], buf, sizeof(buf), 0);

		if (retval <= 0) {
			// ���� ���� �Ǵ� ���� ó��
			if (retval == 0) {
				std::cout << "Client disconnected." << std::endl;
			}
			else {
				//std::cout << "Recv error: " << WSAGetLastError() << std::endl;
			}
		}

		char packetType = static_cast<char>(buf[1]);

		// Ŭ�󿡼� ���� ��Ŷ ó��
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
				cout << "�������� �� ? : " << other.playerID << endl;
				cout << "posx : " << packet.PosX << endl;
				cout << "posy : " << packet.PosY << endl;
				cout << "id : " << packet.player_id << endl;

			}
			break;
		}

		case CS_PLAYER: {
			// �ް� ����ϰ� ������
			CS_PLAYER_PACKET* p = reinterpret_cast<CS_PLAYER_PACKET*>(buf);
			g_characters[p->player_id].Move(p->direction);

			//cout << p->dirY << endl;
			g_characters[p->player_id].Rotate(p->dirY);
			Send_My_Character_Data(p->player_id);

			for (auto& other : g_characters) {
				if (other.playerID == p->player_id) continue;
				{
					lock_guard<mutex> lock(g_character_mutex);
					if (!g_is_accept[other.playerID]) continue;
				}
				SC_0THER_CHARACTER_MOVE_PACKET packet;
				packet.player_id = id;

				packet.packet_size = sizeof(packet);
				packet.packet_type = SC_OTHER_CHARACTER_MOVE;


				packet.PosX = g_characters[id].GetPostionX();
				packet.PosY = g_characters[id].GetPostionY();
				packet.PosZ = g_characters[id].GetPostionZ();
				packet.DirX = g_characters[id].GetForwordVecX();
				packet.DirZ = g_characters[id].GetForwordVecZ();
				packet.yaw = g_characters[id].GetYaw();
				int retval = send(g_clientSocketes[other.playerID],
					reinterpret_cast<const char*>(&packet), sizeof(packet), 0);
			}
			break;
		}

		case CS_MAP_OK: {
			CS_MAP_OK_PACKET* p = reinterpret_cast<CS_MAP_OK_PACKET*>(buf);
			cout << p->player_id << endl;
			{
				lock_guard<mutex> lock(g_character_mutex);
				Send_Enemy_Data(p->player_id);
			}

			cout << "id : " << p->player_id << endl;
			break;
		}

		case CS_BULLET:
		{
			CS_BULLET_PACKET* p = reinterpret_cast<CS_BULLET_PACKET*>(buf);

			if (p->bisFire) 
			{
				g_characters[p->player_id].Shoot();
				// g_characters[p->player_id].bulletPool.pool[0]->Update(1);
				cout << "�������� Ŭ�� ��Ŷ ����" << endl;
				//Send_Bullet_Data(p->player_id);
			}

			/*auto& a = g_characters[p->player_id].bulletPool.GetAllBullets();

			for (auto& b : a) {
				cout << b->IsActive()<<endl;
			}*/



			break;
		}

		default:
			std::cout << "Unknown packet type: " << packetType << std::endl;
			break;
		}
	}
	SocketUtils::Close(g_clientSocketes[id]);
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
	g_enemies[0] = make_shared<Enemy>(15.f, 0.f, 15.f, 0);
	//g_enemies[0]->currentState = EnemyState::Patrol;

	std::unique_ptr<Character> character = make_unique<Character>();



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

	thread eThread{ EnemyThread };

	// �Ѿ� ������
	thread bThread{ BulletTread };


	// 6. Ŭ���̾�Ʈ ���� ��ٸ��� ����
	while (true)
	{
		// 7. Ŭ���̾�Ʈ ���� ��û ����
		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			return 0;
		}
		SocketUtils::SetTcpNoDelay(clientSocket, true);
		// ���� Ŭ���̾�Ʈ ID ����
		int client_id = get_id();

		// ���� ���� ����
		g_is_accept[client_id] = true;

		// ���� ����
		g_clientSocketes[client_id] = clientSocket;

		cout << "Ŭ���̾�Ʈ ����" << endl;
		cout << client_id << endl;

		// 8. ���ο� ������ �����Ͽ� Ŭ���̾�Ʈ ��û ó��
		g_threads[client_id] = thread{ HandleThread, client_id };
	}

	// 9. ��� ������ ����ɶ� ���� ��ٸ�
	for (auto& t : g_threads)
		t.join();
	eThread.join();
	bThread.join();

	// 10. ���ҽ� ����
	SocketUtils::Close(listenSocket);
	SocketUtils::Clear();
}