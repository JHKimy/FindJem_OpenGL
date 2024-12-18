#pragma once
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9000
#define BUFSIZE 8000



constexpr char CS_LOGIN = 1;
constexpr char CS_READY = 2;
constexpr char CS_MOVE = 3;
constexpr char CS_BULLET = 4;

// 맵 정보
constexpr char SC_MAP_DATA = 1;

// 상대 플레이어 정보
constexpr char SC_ADD_PLAYER = 2;
// 오브젝트 정보 

// 총알 정보
//...... 추가


#pragma pack(push, 1)

#pragma pack(pop) 