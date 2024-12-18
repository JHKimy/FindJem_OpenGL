#pragma once
#include "OpenGL/glew.h"                     
#include "OpenGL/freeglut.h"
#include "OpenGL/freeglut_ext.h"
#include "OpenGL/glm/glm.hpp"
#include "OpenGL/glm/ext.hpp"
#include "OpenGL/glm/gtc/matrix_transform.hpp"

// 윈도우 사이즈
extern GLfloat winWidth;
extern GLfloat winHeight;

// 캐릭터 Y축 회전 값
extern float rotateY;

// 캐릭터 전방 벡터
extern glm::vec3 forwardVector;

extern GLuint shaderProgram;