#include "Global.h"

// ������ ������
GLfloat winWidth{ 700 };
GLfloat winHeight{ 700 };

// ĳ���� Y�� ȸ�� ��
float rotateY{ 0.0f };

// ĳ���� ���� ����
// glm::vec3 forwardVector(0.f);



GLuint shaderProgram = 0.f;

// Ŭ���̾�Ʈ<->���� ��� �� ����
NetworkManager networkmanager;

int g_id;

StartPos startPos;

// ���� ĳ���� ���� 
Direction direction;

// ���� ĳ���� ��ġ
 Position position;

 // ������ ���� �Ϸ� �÷���
bool g_bReady = false;

bool g_isOtherCharacter = false;