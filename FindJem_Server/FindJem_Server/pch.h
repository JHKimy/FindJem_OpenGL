#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <vector>
#include <mutex>
#include <ctime>
#include <queue>
#include <array>
#include "MazeGenerator.h"
#include "Character.h"
#include "Enemy.h"
#include "SocketUtils.h"
#include "protocol.h"


using namespace std;
extern std::array<SOCKET, 2>		g_clientSocketes;
extern std::array<bool, 2>			g_is_accept;
extern std::array<thread, 2>		g_threads;
extern std::array<Character, 2>		g_characters;
extern std::array<std::shared_ptr<Enemy>, 1> g_enemies;
extern mutex						g_character_mutex;
extern mutex						g_mutex;
extern mutex						g_EnemyMutex;
extern mutex						g_BulletMutex;
// 미로 데이터
extern std::vector<std::vector<int>> g_mazeMap;
extern std::queue<SC_BULLET_PACKET> g_bulletQueue;
extern int get_id();


// 미로 블록 한개 사이즈
extern struct BlockSize {
	float x, y, z;
}g_blockSize;


extern struct Position {
	float x, y, z;
}g_startPos;