#include "Global.h"

// 윈도우 사이즈
GLfloat winWidth{ 700 };
GLfloat winHeight{ 700 };

// 캐릭터 Y축 회전 값
float rotateY{ 0.0f };

// 캐릭터 전방 벡터
// glm::vec3 forwardVector(0.f);



GLuint shaderProgram = 0.f;

// 클라이언트<->서버 통신 용 변수
NetworkManager networkmanager;

int g_id;

StartPos startPos;

// 서버 캐릭터 방향 
Direction direction;

// 서버 캐릭터 위치
 Position position;

 // 데이터 수신 완료 플래그
bool g_bReady = false;

bool g_isOtherCharacter = false;