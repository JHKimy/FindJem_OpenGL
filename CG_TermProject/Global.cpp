#include "Global.h"

// 윈도우 사이즈
GLfloat winWidth{ 700 };
GLfloat winHeight{ 700 };

// 캐릭터 Y축 회전 값
float rotateY{ 0.0f };

// 캐릭터 전방 벡터
glm::vec3 forwardVector(0.f);



GLuint shaderProgram = 0.f;