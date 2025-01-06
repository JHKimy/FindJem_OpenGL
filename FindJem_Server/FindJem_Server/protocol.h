#pragma once


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9000
#define BUFSIZE 8000



constexpr char CS_LOGIN = 1;
constexpr char CS_READY = 2;
constexpr char CS_MOVE = 3;
constexpr char CS_BULLET = 4;

// �� ����
constexpr char SC_MAP_DATA = 1;

// ��� �÷��̾� ����
constexpr char SC_ADD_PLAYER = 2;
// ������Ʈ ���� 

// �Ѿ� ����
//...... �߰�


#pragma pack(push, 1)

struct CS_PLAYER_PACKET
{
	int packet_type;	// ��Ŷ ����
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

struct SC_MAP_INFO
{
	int packet_type;	// ��Ŷ ����
	std::vector<std::vector<int>> mazeMap;	// �� ����
	
};

#pragma pack(pop) 