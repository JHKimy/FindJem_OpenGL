#pragma once


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9000
#define BUFSIZE 8000



constexpr char CS_LOGIN = 1;
constexpr char CS_READY = 2;
constexpr char CS_PLAYER = 3;
constexpr char CS_BULLET = 4;

// �� ����
constexpr char SC_MAZE_DATA = 1;

// �ʱ� �÷��̾� ���� 
constexpr char SC_INIT_CHARACTER = 2;

// ��� �÷��̾� ����
constexpr char SC_ADD_CHARACTER = 3;
// ������Ʈ ���� 

// �Ѿ� ����
//...... �߰�


#pragma pack(push, 1)

struct CS_PLAYER_PACKET
{
	char packet_size;	// ��Ŷ ũ��
	char packet_type;	// ��Ŷ ����
	int player_id;		// �÷��̾� id
	int player_hp;		// hp
	bool status = true;	// ����
	bool ready = false;	// ���� ����
	char direction;		// 0 : forward  / 1 : back / 2 : left / 3 : right / 4 : jump
	
	float dirX;
	float dirY;
	float dirZ;

	float camera_angleX;
	float camera_angleY;

};

struct SC_MAZE_INFO
{
	char packet_size;		// ��Ŷ ũ��
	char packet_type;		// ��Ŷ ����
	int mazeMap[15][15];	// �� ����
	int player_id;			// �÷��̾� id
};
struct SC_MY_CHARACTER
{
	char packet_size;		// ��Ŷ ũ��
	char packet_type;		// ��Ŷ ����
	float x, y, z;			// �ʱ� �÷��̾� ��ġ ���� ��Ŷ
};

#pragma pack(pop) 