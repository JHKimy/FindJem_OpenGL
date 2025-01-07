#pragma once
#include "OpenGL/glew.h"                     
#include "OpenGL/freeglut.h"
#include "OpenGL/freeglut_ext.h"
#include "OpenGL/glm/glm.hpp"
#include "OpenGL/glm/ext.hpp"
#include "OpenGL/glm/gtc/matrix_transform.hpp"

// ������ ������
extern GLfloat winWidth;
extern GLfloat winHeight;

// ĳ���� Y�� ȸ�� ��
extern float rotateY;

// ĳ���� ���� ����
extern glm::vec3 forwardVector;

extern GLuint shaderProgram;

// Ŭ���̾�Ʈ �ĺ� id
extern int g_id;

extern struct StartPos {
	float x, y, z;
}startPos;