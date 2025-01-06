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

struct CS_PLAYER_PACKET
{
	int packet_type;	// 패킷 종류
	int player_id;		// 플레이어 id
	int player_hp;		// hp
	bool status = true;	// 상태
	bool ready = false;	// 레디 상태
	char direction;		// 0 : forward  / 1 : back / 2 : left / 3 : right / 4 : jump
	
	float dirX;
	float dirY;
	float dirZ;

	float camera_angleX;
	float camera_angleY;

};

struct SC_MAP_INFO
{
	int packet_type;	// 패킷 종류
	std::vector<std::vector<int>> mazeMap;	// 맵 정보
	
};

#pragma pack(pop) 