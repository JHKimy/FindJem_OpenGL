#pragma once
#include <winSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <iostream>
#include <cstring>
#include <thread>
#include <queue>
#include <mutex>
#include <memory>
#include "../FindJem_Server/FindJem_Server/protocol.h"
using namespace std;

class Scene;

#pragma comment(lib, "ws2_32.lib")
class NetworkManager
{

public:
    NetworkManager();

    ~NetworkManager();

    bool Connect();

    bool RecvThread();

    void SendReady();

    void SendPlayerMove(CS_PLAYER_PACKET& p);

    void SetScene(std::shared_ptr<Scene> Scene);

    SOCKET GetSocket() { return clientSocket; }

    void RecvMazeData();

    void RecvEnemiesData();

    void RecvBulletData();

private:
    SOCKET clientSocket;

    std::shared_ptr<Scene> m_Scene;
};
