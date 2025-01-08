#pragma once
#include "OpenGL/glew.h"                     
#include "OpenGL/freeglut.h"
#include "OpenGL/freeglut_ext.h"
#include "OpenGL/glm/glm.hpp"
#include "OpenGL/glm/ext.hpp"
#include "OpenGL/glm/gtc/matrix_transform.hpp"

#include "NetworkManager.h"


// 윈도우 사이즈
extern GLfloat winWidth;
extern GLfloat winHeight;

// 캐릭터 Y축 회전 값
extern float rotateY;

// 캐릭터 전방 벡터
extern glm::vec3 forwardVector;

extern GLuint shaderProgram;

// 클라이언트<->서버 통신 용 변수
extern NetworkManager networkmanager;

// 클라이언트 식별 id
extern int g_id;

// 서버 캐릭터 시작 위치
extern struct StartPos {
	float x, y, z;
}startPos;

// 서버 캐릭터 방향
extern struct Direction {
	float x, y, z;
}direction;

// 서버 캐릭터 위치
extern struct Position {
	float x, y, z;
}position;

// 데이터 수신 완료 플래그
extern bool g_bReady;

extern bool g_isOtherCharacter;

extern bool g_keyPressed;