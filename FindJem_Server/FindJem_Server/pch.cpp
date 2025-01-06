#include "pch.h"

std::array<SOCKET, 3>   g_clientSocketes;
std::array<bool, 3>      g_is_accept;
mutex               g_mutex;
int get_id()
{
    g_mutex.lock();
    for (int i = 0; i < 3; ++i) {
        if (!g_is_accept[i]) {
            g_mutex.unlock();
            return i;
        }
    }
    g_mutex.unlock();
    return -1;
}