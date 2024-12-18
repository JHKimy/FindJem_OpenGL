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
#include "../FindJem_Server/FindJem_Server/protocol.h"
using namespace std;

#pragma comment(lib, "ws2_32.lib")
class NetworkManager
{
public:
    NetworkManager();

    ~NetworkManager();

    bool Connect(const char* ipaddress);

   
private:
    SOCKET clientSocket;
};

