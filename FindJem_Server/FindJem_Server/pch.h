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
#include "SocketUtils.h"
#include "protocol.h"


using namespace std;
extern std::array<SOCKET, 3>   g_clientSocketes;
extern std::array<bool, 3>      g_is_accept;
extern mutex               g_mutex;
extern int get_id();

