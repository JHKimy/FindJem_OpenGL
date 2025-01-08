#pragma once
#include "OpenGL/glew.h"                     
#include "OpenGL/freeglut.h"
#include "OpenGL/freeglut_ext.h"
#include "OpenGL/glm/glm.hpp"
#include "OpenGL/glm/ext.hpp"
#include "OpenGL/glm/gtc/matrix_transform.hpp"

#include "NetworkManager.h"


// ������ ������
extern GLfloat winWidth;
extern GLfloat winHeight;

// ĳ���� Y�� ȸ�� ��
extern float rotateY;

// ĳ���� ���� ����
extern glm::vec3 forwardVector;

extern GLuint shaderProgram;

// Ŭ���̾�Ʈ<->���� ��� �� ����
extern NetworkManager networkmanager;

// Ŭ���̾�Ʈ �ĺ� id
extern int g_id;

// ���� ĳ���� ���� ��ġ
extern struct StartPos {
	float x, y, z;
}startPos;

// ���� ĳ���� ����
extern struct Direction {
	float x, y, z;
}direction;

// ���� ĳ���� ��ġ
extern struct Position {
	float x, y, z;
}position;

// ������ ���� �Ϸ� �÷���
extern bool g_bReady;

extern bool g_isOtherCharacter;

extern bool g_keyPressed;