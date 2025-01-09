#pragma once


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9000
#define BUFSIZE 8000



constexpr char CS_LOGIN = 1;
constexpr char CS_READY = 2;
constexpr char CS_PLAYER = 3;
constexpr char CS_BULLET = 4;



// 맵 정보
constexpr char SC_MAZE_DATA = 1;

// 초기 플레이어 정보 
// 상대 플레이어 정보
constexpr char SC_ADD_CHARACTER = 2;
constexpr char SC_CHARACTER_MOVE = 3;
constexpr char SC_OTHER_CHARACTER_MOVE = 4;

constexpr char SC_ENEMY = 4;

// 오브젝트 정보 

// 총알 정보
//...... 추가


#pragma pack(push, 1)

// 클라->서버 패킷
struct CS_PLAYER_PACKET
{
	char packet_size;	// 패킷 크기
	char packet_type;	// 패킷 종류
	int player_id;		// 플레이어 id
	int player_hp;		// hp
	bool status = true;	// 상태
	bool ready = false;	// 레디 상태
	char direction = -1;		// 0 : forward  / 1 : back / 2 : left / 3 : right / 4 : jump
	
	float dirX;
	float dirY;
	float dirZ;

	float camera_angleX;
	float camera_angleY;

};

struct CS_READY_PACKET
{
	char packet_size;		// 패킷 크기
	char packet_type;		// 패킷 종류
	int player_id;			// 플레이어 id
};

// 서버->클라 패킷
struct SC_MAZE_INFO
{
	char packet_size;		// 패킷 크기
	char packet_type;		// 패킷 종류
	int mazeMap[15][15];	// 맵 정보
	int player_id;			// 플레이어 id
	float x, y, z;			// 초기 플레이어 위치 정보 패킷
};

struct SC_ADD_CHARACTER_PACKET
{
	char packet_size;	// 패킷 크기
	char packet_type;	// 패킷 종류
	int player_id;		// 플레이어 id
	int player_hp;		// hp

	float PosX;
	float PosY;
	float PosZ;

	float camera_angleX;
	float camera_angleY;

};

struct SC_CHARACTER_MOVE_PACKET
{
	char packet_size;	// 패킷 크기
	char packet_type;	// 패킷 종류
	int player_id;		// 플레이어 id
	
	float PosX;
	float PosY;
	float PosZ;
	
	float DirX;
	float DirZ;

	float yaw;
};

struct SC_0THER_CHARACTER_MOVE_PACKET
{
	char packet_size;	// 패킷 크기
	char packet_type;	// 패킷 종류
	int player_id;		// 플레이어 id

	float PosX;
	float PosY;
	float PosZ;

	float DirX;
	float DirZ;

	float yaw;
};

struct SC_ENEMY_PACKET {
	char packet_size;	// 패킷 크기
	char packet_type;	// 패킷 종류
	int player_id;		// 플레이어 id

	float PosX;
	float PosY;
	float PosZ;

};

#pragma pack(pop) 