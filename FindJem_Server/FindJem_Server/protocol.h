#pragma once


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9000
#define BUFSIZE 8192


// Ŭ�󼭹� Ÿ��
constexpr char CS_LOGIN = 1;
constexpr char CS_READY = 2;
constexpr char CS_PLAYER = 3;
constexpr char CS_BULLET = 4;
constexpr char CS_MAP_OK = 5;


// ����Ŭ�� Ÿ��
constexpr char SC_MAZE_DATA = 1;
constexpr char SC_ADD_CHARACTER = 2;
constexpr char SC_CHARACTER_MOVE = 3;
constexpr char SC_OTHER_CHARACTER_MOVE = 4;
constexpr char SC_ENEMY = 5;
constexpr char SC_BULLET = 6;



#pragma pack(push, 1)

// Ŭ��->���� ��Ŷ
struct CS_PLAYER_PACKET
{
	char packet_size;	// ��Ŷ ũ��
	char packet_type;	// ��Ŷ ����
	int player_id;		// �÷��̾� id
	int player_hp;		// hp
	bool status = true;	// ����
	bool ready = false;	// ���� ����
	char direction = -1;		// 0 : forward  / 1 : back / 2 : left / 3 : right / 4 : jump

	float dirX;
	float dirY;
	float dirZ;

	float camera_angleX;
	float camera_angleY;

};

struct CS_READY_PACKET
{
	char packet_size;		// ��Ŷ ũ��
	char packet_type;		// ��Ŷ ����
	int player_id;			// �÷��̾� id
};

struct CS_MAP_OK_PACKET
{
	char packet_size;		// ��Ŷ ũ��
	char packet_type;		// ��Ŷ ����
	int player_id;			// �÷��̾� id
};

struct CS_BULLET_PACKET
{
	char	packet_size;		// ��Ŷ ũ��
	char	packet_type;		// ��Ŷ ����
	int		player_id;			// �÷��̾� id
	bool	bisFire = false;	// �߻�
};


// ����->Ŭ�� ��Ŷ
struct SC_MAZE_INFO
{
	char packet_size;		// ��Ŷ ũ��
	char packet_type;		// ��Ŷ ����
	int mazeMap[15][15];	// �� ����
	int player_id;			// �÷��̾� id
	float x, y, z;			// �ʱ� �÷��̾� ��ġ ���� ��Ŷ
};

struct SC_ADD_CHARACTER_PACKET
{
	char packet_size;	// ��Ŷ ũ��
	char packet_type;	// ��Ŷ ����
	int player_id;		// �÷��̾� id
	int player_hp;		// hp

	float PosX;
	float PosY;
	float PosZ;

	float camera_angleX;
	float camera_angleY;

};

struct SC_CHARACTER_MOVE_PACKET
{
	char packet_size;	// ��Ŷ ũ��
	char packet_type;	// ��Ŷ ����
	int player_id;		// �÷��̾� id

	float PosX;
	float PosY;
	float PosZ;

	float DirX;
	float DirZ;

	float yaw;
};

struct SC_0THER_CHARACTER_MOVE_PACKET
{
	char packet_size;	// ��Ŷ ũ��
	char packet_type;	// ��Ŷ ����
	int player_id;		// �÷��̾� id

	float PosX;
	float PosY;
	float PosZ;

	float DirX;
	float DirZ;

	float yaw;
};

struct SC_ENEMY_PACKET {
	char	packet_size;	// ��Ŷ ũ��
	char	packet_type;	// ��Ŷ ����
	int		enemy_id;		// �� id
	bool	bActive;

	float	PosX;
	float	PosY;
	float	PosZ;

};

struct SC_BULLET_PACKET 
{
	char	packet_size;	// ��Ŷ ũ��
	char	packet_type;	// ��Ŷ ����
	int		player_id;
	int		bullet_id;
	bool	bActive;
	float	PosX;
	float	PosY;
	float	PosZ;
};

#pragma pack(pop) 