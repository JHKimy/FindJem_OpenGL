#include "pch.h"
#include "AstarPath.h"
#include <random>

void HandleBulletEnemyCollisions()
{
	// 모든 캐릭터에 대해서
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
					enemy->TakeDamage(10, bulletDirection); // 적 체력 감소
					bullet->DeActivate();                 // 총알 비활성화

					printf("Enemy hit\n");

					if (!enemy->IsActive())
					{
						printf("Enemy defeated\n");
						g_characters[i].IncrementDefeatedEnemies(); // 적 제거 카운트 증가
						break;
					}
				}
			}
		}
	}
}

void HandleCharacterEnemyCollisions() 
{
	float currentTime = static_cast<float>(std::chrono::duration_cast<std::chrono::seconds>(
		std::chrono::steady_clock::now().time_since_epoch())
		.count());

	// 모든 캐릭터에 대해 처리
	for (int i = 0; i < g_characters.size(); ++i) {
		auto& character = g_characters[i];

		if (!character.GetActive()) continue; // 캐릭터가 비활성 상태면 건너뜀

		for (auto& enemy : g_enemies) {
			if (!enemy->IsActive()) continue; // 적이 비활성 상태면 건너뜀

			// 캐릭터와 적 간 거리 계산
			float distance = V::Distance(character.GetPosition(), enemy->GetPosition());

			// 충돌 조건 확인
			if (distance < (character.boundingRadius + enemy->boundingRadius +1.f)) {
				printf("Character and Enemy collided\n");

				// 체력 감소
				vec3 tempDis =
					vec3(character.GetPosition().x - enemy->GetPosition().x,
						character.GetPosition().y - enemy->GetPosition().y,
						character.GetPosition().z - enemy->GetPosition().z);

				vec3 enemyToCharacter = V::Normalize(tempDis);

				if (character.CanTakeDamage(currentTime)) {
					character.TakeDamage(10); // 체력 10 감소
					character.SetLastDamageTime(currentTime);

					printf("Character health: %d\n", character.GetHealth());
				}

				// 뒤로 밀리기
				// 밀림 거리 1

				float dis = 0.05f;

				vec3 knockbackDirection =
					vec3(enemyToCharacter.x * dis,
						enemyToCharacter.y * dis,
						enemyToCharacter.z * dis
						);
				vec3 knockbackMove =
					vec3(character.GetPosition().x + knockbackDirection.x,
						character.GetPosition().y + knockbackDirection.y,
						character.GetPosition().z + knockbackDirection.z);

				character.SetPosition(knockbackMove);

				// 충돌 처리 후 상태 출력
				printf("Character health: %d\n", character.GetHealth());
				break;
			}
		}
	}
}



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
	p.yaw = g_characters[clientid].GetYaw();

	p.DirX = g_characters[clientid].GetForwordVecX();
	p.DirZ = g_characters[clientid].GetForwordVecZ();

	if (g_is_accept[clientid]) {
		// 데이터를 클라이언트 소켓으로 전송
		int retval = send(g_clientSocketes[clientid],
			reinterpret_cast<const char*>(&p), sizeof(p), 0);

	}
}

void Send_Enemy_Data(int clientid)
{

	// 적 정보 담는 패킷 구조체
	SC_START_ENEMY_PACKET p;
	p.packet_size = sizeof(p);
	p.packet_type = SC_START_ENEMY;
	for (int i{}; i < g_enemies.size(); ++i) 
	{
		p.data[i].id = g_enemies[i]->GetEnemyID();
		p.data[i].isActive = true;
		p.data[i].x = g_enemies[i]->GetPostionX();
		p.data[i].y = g_enemies[i]->GetPostionY();
		p.data[i].z = g_enemies[i]->GetPostionZ();

		// cout << "id : " << p.data[i].id << endl;
		//cout << "send ?? " << endl;
	}

	if (g_is_accept[clientid]) {
		//cout << " send enemy to clientid : " << clientid << endl;
		// 데이터를 클라이언트 소켓으로 전송
		int retval = send(g_clientSocketes[clientid],
			reinterpret_cast<const char*>(&p), sizeof(p), 0);

		if (retval == SOCKET_ERROR) {
			//cout << "fail ! " << clientid << ": " << WSAGetLastError() << endl;
		}
		else {
			cout << "Send to client " << clientid << endl;
		}

	}

	//p.player_id = clientid;
	//p.bullet_id = i;
	//p.PosX = bullets[i]->GetPosition().x;
	//p.PosY = bullets[i]->GetPosition().y;
	//p.PosZ = bullets[i]->GetPosition().z;




}


void Send_Bullet_Data(int clientid)
{

	
	auto& bullets = g_characters[clientid].bulletPool.pool;

	for (int i{}; i< bullets.size();++i)
	{
		if (bullets[i]->IsActive()) 
		{
			// 적 정보 담는 패킷 구조체
			
		
			for (auto& other : g_characters) {
				SC_BULLET_PACKET p;
				//if (other.playerID == clientid) continue;
				{
					lock_guard<mutex> lock(g_character_mutex);
					if (!g_is_accept[other.playerID]) continue;
				}

				p.packet_size = sizeof(p);
				p.packet_type = SC_BULLET;
				p.player_id = clientid;
				p.bullet_id = i;
				p.PosX = bullets[i]->GetPosition().x;
				p.PosY = bullets[i]->GetPosition().y;
				p.PosZ = bullets[i]->GetPosition().z;
				if (g_is_accept[other.playerID]) {
					//cout << " 어떤 클라에게 감? : " << other.playerID << endl;
					// 데이터를 클라이언트 소켓으로 전송
					int retval = send(g_clientSocketes[other.playerID],
						reinterpret_cast<const char*>(&p), sizeof(p), 0);

				}
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
	while (true) {
		if (g_characters[0].isReady) {
			{
				std::lock_guard<std::mutex> lock(g_EnemyMutex);

				for (int i = 0; i < g_characters.size(); ++i) 
				{





					for (int j = 0; j < g_enemies.size(); ++j) 
					{
						if (g_is_accept[i]) {
							float distanceToPlayer = g_enemies[j]->DistanceToPlayer(j, i);

							// 클라이언트별 detectPath를 확인
							if (g_enemies[j]->detectPath.find(i) == g_enemies[j]->detectPath.end()) {
								g_enemies[j]->detectPath[i] = false; // 초기화
							}

							switch (g_enemies[j]->currentState) {
							case EnemyState::Patrol:
								if (g_enemies[j]->detectionRadius > distanceToPlayer) {
									g_enemies[j]->currentState = EnemyState::Chase;
								}
								else {
									g_enemies[j]->Patrol(g_mazeMap);
								}
								break;

							case EnemyState::Chase:
								if (!g_enemies[j]->detectPath[i]) {
									g_enemies[j]->MakeAStarPath(j, i); // 클라이언트 i에 대한 경로 생성
									g_enemies[j]->detectPath[i] = true;
								}
								g_enemies[j]->Chase();
								break;
							}
						}
					}
				}

				// **캐릭터와 적 충돌 감지 및 처리**
				HandleCharacterEnemyCollisions();
			}

			for (int i = 0; i < g_characters.size(); ++i) {
				for (int j = 0; j < g_enemies.size(); ++j) {
					if (g_is_accept[i]) {

						

						SC_ENEMY_PACKET p;
						p.packet_size = sizeof(p);
						p.packet_type = SC_ENEMY;

						p.enemy_id = g_enemies[j]->GetEnemyID();
						p.bActive = g_enemies[j]->IsActive();

						p.PosX = g_enemies[j]->GetPostionX();
						p.PosY = g_enemies[j]->GetPostionY();
						p.PosZ = g_enemies[j]->GetPostionZ();


						int retval = send(g_clientSocketes[i], reinterpret_cast<const char*>(&p), sizeof(p), 0);
						if (retval == SOCKET_ERROR) {
							std::cerr << "Send failed for client " << i << ": " << WSAGetLastError() << std::endl;
						}
					}
				}
			}

			std::this_thread::sleep_for(std::chrono::microseconds(50));
		}
	}
}

//void BulletTread()
//{
//	// 서버에서 클라로 보내는 패킷
//	SC_BULLET_PACKET p;
//	p.packet_size = sizeof(p);
//	p.packet_type = SC_BULLET;
//
//	while (true) {
//		//std::this_thread::sleep_for(std::chrono::milliseconds(500)); // CPU 과부하 방지
//
//		std::lock_guard<std::mutex> lock(g_BulletMutex); // Mutex로 보호
//		if (g_characters[0].isReady) {
//			for (int i{}; i < g_characters.size(); ++i)
//			{
//				for (int j{};j<g_characters[i].bulletPool.pool.size();++j)
//				{
//					if (g_is_accept[i]) 
//					{
//						if (g_characters[i].bulletPool.pool[j]->IsActive()) 
//						{
//							//g_characters[i].bulletPool.pool[j]->Update();
//
//							g_characters[i].bulletPool.UpdateAllBullets
//							(vec3(g_characters[i].GetPostionX(), g_characters[i].GetPostionY(), g_characters[i].GetPostionZ()));
//
//
//							
//							p.player_id = i;
//							p.bullet_id = j;
//							p.PosX = g_characters[i].bulletPool.pool[j]->GetPosition().x;
//							p.PosY = g_characters[i].bulletPool.pool[j]->GetPosition().y + 1.f;
//							p.PosZ = g_characters[i].bulletPool.pool[j]->GetPosition().z;
//							p.bActive = g_characters[i].bulletPool.pool[j]->IsActive();
//
//
//							HandleBulletEnemyCollisions();
//
//							std::this_thread::sleep_for(std::chrono::microseconds(1)); // 10ms 대기
//
//							//std::lock_guard<std::mutex> lock(g_BulletMutex);
//							//cout << "id : " << p.player_id << endl;
//							//cout << "bullet : " << p.PosX << " , " << p.PosZ << endl;
//
//							int retval = send(g_clientSocketes[i],
//								reinterpret_cast<const char*>(&p), sizeof(p), 0);
//							if (retval == SOCKET_ERROR) {
//								std::cerr << "[ERROR] Send failed for Player " << i
//									<< " with error: " << WSAGetLastError() << "\n";
//							}
//							else {
//								std::cerr << "[SUCCESS] Packet sent successfully to Player " << i << "\n";
//							}
//						}
//					}
//				}
//			}
//		}
//
//	}
//}

void BulletTread() {
	while (true) {
		{
			lock_guard<mutex> lock(g_BulletMutex); // Mutex 보호
			if (g_characters.empty() || !g_characters[0].isReady) continue;

			// 모든 플레이어에 대해 처리
			for (auto& character : g_characters) {
				int senderID = character.playerID; // 현재 플레이어 ID

				// 모든 총알에 대해 처리
				for (int j = 0; j < character.bulletPool.pool.size(); ++j) {
					if (!character.bulletPool.pool[j]->IsActive()) continue;

					//g_characters[i].bulletPool.pool[j]->Update();

					character.bulletPool.UpdateAllBullets
					(vec3(character.GetPostionX(), character.GetPostionY(), character.GetPostionZ()));
					// 총알 정보 생성
					SC_BULLET_PACKET packet;
					packet.packet_size = sizeof(packet);
					packet.packet_type = SC_BULLET;
					packet.player_id = senderID;
					packet.bullet_id = j;
					packet.PosX = character.bulletPool.pool[j]->GetPosition().x;
					packet.PosY = character.bulletPool.pool[j]->GetPosition().y + 1.f;
					packet.PosZ = character.bulletPool.pool[j]->GetPosition().z;
					packet.bActive = character.bulletPool.pool[j]->IsActive();
					HandleBulletEnemyCollisions();
					// 다른 모든 클라이언트에게 전송
					for (auto& other : g_characters) {
						//if (other.playerID == senderID) continue; // 자기 자신 제외

						//{
						//	lock_guard<mutex> charLock(g_character_mutex);
						//	if (!g_is_accept[other.playerID]) continue; // 연결되지 않은 플레이어 제외
						//}

						int retval = send(g_clientSocketes[other.playerID],
							reinterpret_cast<const char*>(&packet), sizeof(packet), 0);
						//if (retval == SOCKET_ERROR) {
						//	/*cerr << "[ERROR] Send failed for Player " << other.playerID
						//		<< " with error: " << WSAGetLastError() << endl;*/
						//}
						//else {
						//	cerr << "[SUCCESS] Bullet packet sent to Player " << other.playerID
						//		<< " from Player " << senderID << endl;
						//}
					}
				}
			}
		}

		// CPU 과부하 방지
		this_thread::sleep_for(chrono::microseconds(1000)); // 100µs 대기
	}
}





// 클라 요청 처리하는 서버 스레드 함수
void HandleThread(int id)
{
	// 서버 캐릭터 생성
	Character character{ id };
	{
		lock_guard<mutex> lock(g_character_mutex);
		g_characters[id] = character;
		g_characters[id].playerID = id;
	}

	// 미로 데이터 보내기
	Send_Maze_Data(id);


	while (true)
	{
		char buf[BUFSIZE];

		// 패킷 전체 데이터 수신
		int retval = recv(g_clientSocketes[id], buf, sizeof(buf), 0);

		if (retval <= 0) {
			// 연결 종료 또는 오류 처리
			if (retval == 0) {
				std::cout << "Client disconnected." << std::endl;
			}
			else {
				//std::cout << "Recv error: " << WSAGetLastError() << std::endl;
			}
		}

		char packetType = static_cast<char>(buf[1]);

		// 클라에서 받은 패킷 처리
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
				//cout << "누구에게 감 ? : " << other.playerID << endl;
				//cout << "posx : " << packet.PosX << endl;
				//cout << "posy : " << packet.PosY << endl;
				//cout << "id : " << packet.player_id << endl;

			}
			break;
		}

		case CS_PLAYER: {
			// 받고 계산하고 보내기
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
			//cout << p->player_id << endl;
			{
				lock_guard<mutex> lock(g_character_mutex);
				Send_Enemy_Data(p->player_id);
			}

			//cout << "id : " << p->player_id << endl;
			break;
		}

		case CS_BULLET:
		{
			CS_BULLET_PACKET* p = reinterpret_cast<CS_BULLET_PACKET*>(buf);

			if (p->bisFire) 
			{
				g_characters[p->player_id].Shoot();
				// g_characters[p->player_id].bulletPool.pool[0]->Update(1);
				//cout << "서버에서 클릭 패킷 받음" << endl;
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
	// 미로 생성기
	std::unique_ptr<MazeGenerator> mazeGenerator;
	mazeGenerator = make_unique<MazeGenerator>(7, 7);

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





	std::vector<vec3> emptySpaces;

	// 빈 공간 찾기
	for (int z = 0; z < g_mazeMap.size(); ++z)
	{
		for (int x = 0; x < g_mazeMap[z].size(); ++x)
		{
			if (g_mazeMap[z][x] == 0)
			{
				emptySpaces.push_back(vec3(x * g_blockSize.x, 0.0f, z * g_blockSize.z));
			}
		}
	}

	// 랜덤으로 적 생성
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, emptySpaces.size() - 1);

	for (int i = 0; i < g_enemies.size(); ++i)
	{
		int randomIndex = dist(gen);
		vec3 enemyPosition = emptySpaces[randomIndex];
		g_enemies[i] = std::make_shared<Enemy>(enemyPosition.x, enemyPosition.y, enemyPosition.z,i);
		emptySpaces.erase(emptySpaces.begin() + randomIndex);
	}




	//g_enemies[0] = make_shared<Enemy>(15.f, 0.f, 15.f, 0);
	//g_enemies[0]->currentState = EnemyState::Patrol;

	std::unique_ptr<Character> character = make_unique<Character>();



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

	thread eThread{ EnemyThread };

	// 총알 스레드
	thread bThread{ BulletTread };


	// 6. 클라이언트 접속 기다리는 루프
	while (true)
	{
		// 7. 클라이언트 접속 요청 수락
		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			return 0;
		}
		SocketUtils::SetTcpNoDelay(clientSocket, true);
		// 고유 클라이언트 ID 생성
		int client_id = get_id();

		// 연결 상태 설정
		g_is_accept[client_id] = true;

		// 소텟 저장
		g_clientSocketes[client_id] = clientSocket;

		cout << "클라이언트 접속" << endl;
		cout << client_id << endl;

		// 8. 새로운 스레드 생성하여 클라이언트 요청 처리
		g_threads[client_id] = thread{ HandleThread, client_id };
	}

	// 9. 모든 스레드 종료될때 까지 기다림
	for (auto& t : g_threads)
		t.join();
	eThread.join();
	bThread.join();

	// 10. 리소스 정리
	SocketUtils::Close(listenSocket);
	SocketUtils::Clear();
}