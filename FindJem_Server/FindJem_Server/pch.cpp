#include "pch.h"

std::array<SOCKET, 3>   g_clientSocketes;
std::array<bool, 3>      g_is_accept;
std::array<thread, 3>      g_threads;
std::array<Character, 3>	g_characters;
// 미로 데이터
std::vector<std::vector<int>> g_mazeMap;

std::array<std::shared_ptr<Enemy>, 1> g_enemies;
// 멀티스레드 환경에서 동기화를 위한 mutex
mutex						g_character_mutex;
mutex                       g_mutex;

// 빈 슬롯의 클라이언트 ID 반환 함수
int get_id()
{
    g_mutex.lock(); // mutex 잠금
    for (int i = 0; i < 3; ++i) {
        if (!g_is_accept[i]) {  // 비어 있으면
            g_mutex.unlock();   // mutex 해제
            return i;
        }
    }
    g_mutex.unlock();   // 모두 차 있으면 mutex 해제
    return -1;
}

// 미로 블럭 1개 사이즈
struct BlockSize g_blockSize = { 5.f,3.f,5.f };

struct Position g_startPos;