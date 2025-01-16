#include "pch.h"

std::array<SOCKET, 3>   g_clientSocketes;
std::array<bool, 3>      g_is_accept;
std::array<thread, 3>      g_threads;
std::array<Character, 3>	g_characters;
// �̷� ������
std::vector<std::vector<int>> g_mazeMap;

std::array<std::shared_ptr<Enemy>, 1> g_enemies;
// ��Ƽ������ ȯ�濡�� ����ȭ�� ���� mutex
mutex						g_character_mutex;
mutex                       g_mutex;

// �� ������ Ŭ���̾�Ʈ ID ��ȯ �Լ�
int get_id()
{
    g_mutex.lock(); // mutex ���
    for (int i = 0; i < 3; ++i) {
        if (!g_is_accept[i]) {  // ��� ������
            g_mutex.unlock();   // mutex ����
            return i;
        }
    }
    g_mutex.unlock();   // ��� �� ������ mutex ����
    return -1;
}

// �̷� �� 1�� ������
struct BlockSize g_blockSize = { 5.f,3.f,5.f };

struct Position g_startPos;