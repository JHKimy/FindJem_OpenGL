﻿#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;



#include "Camera.h"
#include "Light.h"
#include "Actor.h"
#include "Bullet.h"
#include "Character.h"



bool command_w	{ false };
bool command_a	{ false };
bool command_s	{ false };
bool command_d	{ false };
bool command_1	{ true };
bool command_2	{ false };



// 메인 카메라 생성
Camera mainCamera(
	glm::vec3(10.0f, 10.0f, 10.0f),		// pos
	glm::vec3(0.0f, 0.0f, 0.0f),		// target
	glm::vec3(0.0f, 1.0f, 0.0f));		// up

Light mainLight(
	glm::vec3(0.0f, 10.0f, 10.0f),		// pos
	glm::vec3(1.0f, 1.0f, 1.0f));		// color



// 윈도우 사이즈
GLfloat winWidth{ 700 }, winHeight{ 700 };



// 콜백 함수
GLvoid drawScene();
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
GLvoid Motion(int x, int y);
GLvoid TimerFunction(int value);
GLvoid PassiveMotion(int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);

// 셰이더
GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;


// ===== 셰이더 만들기 ===== 

char* filetobuf(string file)
{
	ifstream in(file);

	string* str_buf = new string;
	while (in) {
		string temp;
		getline(in, temp);
		str_buf->append(temp);
		str_buf->append("\n");
	}

	return const_cast<char*>(str_buf->c_str());
}

bool make_vertexShaders()
{
	GLchar* vertexSource;

	vertexSource = filetobuf("vertex.glsl");

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		return false;
	}
}

bool make_fragmentShaders()
{
	GLchar* fragmentSource;
	//--- 프래그먼트 세이더 읽어 저장하고 컴파일하기
	fragmentSource = filetobuf("fragment.glsl"); // 프래그세이더 읽어오기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
		return false;
	}
}

GLuint make_shaderProgram()
{
	make_vertexShaders();

	make_fragmentShaders();

	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram();
	glAttachShader(ShaderProgramID, vertexShader);
	glAttachShader(ShaderProgramID, fragmentShader);
	glLinkProgram(ShaderProgramID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];

	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program 연결 실패\n" << errorLog << endl;
		return false;
	}
	glUseProgram(ShaderProgramID);
	return ShaderProgramID;
}

// ========================= 






// 객체
Actor* test;
Character* character;


void main(int argc, char** argv)
{
	// 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Find Jem");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();

	// 은면 제거
	//glEnable(GL_DEPTH_TEST);
	//glShadeModel(GL_SMOOTH);
	//glEnable(GL_DITHER);        // 표면을 매끄럽게

// ===============================================================


	

	// 셰이더 프로그램 생성
	shaderProgram = make_shaderProgram();

	// Actor 객체 생성
	test = new Actor(
		"Cube.obj", 
		glm::vec3(0), 
		glm::vec3(1), 
		glm::vec3(0), 
		glm::vec3(1, 0, 0));
	
	character = new Character(
		"Boss.obj",
		glm::vec3(2.f,0.f,0.f),
		glm::vec3(.1f),
		glm::vec3(0),
		glm::vec3(0, 1, 0),
		0.05f,
		100
	);


	// 깊이 테스트 활성화
	glEnable(GL_DEPTH_TEST);















//=========================
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutTimerFunc(17, TimerFunction, 1);
	glutMotionFunc(Motion);
	glutPassiveMotionFunc(PassiveMotion);
	glutMouseFunc(Mouse);
	glutSpecialFunc(SpecialKeyboard);

	glutMainLoop();
//=========================
}







GLvoid drawScene()
{
	// 백 버퍼에 컬러 설정
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// 컬러 버퍼와 깊이 버퍼 초기화
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//============================================================

	// 셰이더 사용해서 렌더링
	// glUseProgram(shaderProgram);

	// 카메라 설정
	mainCamera.ApplyCamera(shaderProgram);
	mainCamera.SwitchToFirstPerson(character->GetPosition()+10.f,character->GetDirection());


	test->Render(shaderProgram);
	character->Render(shaderProgram);


	// 조명
	glm::vec3 cameraPosition = mainCamera.GetPosition();
	mainLight.ApplyLighting(shaderProgram, cameraPosition);
	


	// 전면 버퍼와 후면 버퍼 스왑
	glutSwapBuffers();
	glutPostRedisplay();

}



GLvoid Keyboard(unsigned char key, int x, int y)
{
	glm::vec3 moveDir(0.0f);

	switch (key)
	{
	case 'w': command_w = true; break;		// 앞으로 이동
	case 's': command_s = true; break;		// 뒤로 이동
	case 'a': command_a = true; break;		// 왼쪽으로 이동
	case 'd': command_d = true; break;		// 오른쪽으로 이동
	case 32:								// 스페이스바 (점프)
		character->Jump();
		return;

	case '1':
		break;
	}

	// 방향 설정
	character->SetDirection(moveDir);

	glutPostRedisplay();
}

GLvoid KeyboardUp(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		command_w = false;
		break;
	case 's':
		command_s = false;
		break;
	case 'a':
		command_a = false;
		break;
	case 'd':
		command_d = false;
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}

	glutPostRedisplay();
}

GLvoid Motion(int x, int y)
{
	return GLvoid();
}

GLvoid TimerFunction(int value)
{
	float deltaTime = 0.016f; // 약 60FPS 기준

	// 캐릭터 이동 업데이트
	glm::vec3 direction = character->GetDirection();
	if (direction != glm::vec3(0.0f)) 
	{
		// test2->Move(direction);
	}

	if (command_w) {
		glm::vec3 dir{ 0, 0, 1 };
		character->Move(dir);
	}

	if (command_s) {
		glm::vec3 dir{ 0, 0, -1 };
		character->Move(dir);
	}

	if (command_a) {
		glm::vec3 dir{ 1, 0, 0 };
		character->Move(dir);
	}

	if (command_d) {
		glm::vec3 dir{ -1, 0, 0 };
		character->Move(dir);
	}
	// 캐릭터 상태 업데이트 (중력 등)
	character->Update(deltaTime);

	// 다음 프레임 요청
	glutTimerFunc(16, TimerFunction, 1);
	glutPostRedisplay();
}

GLvoid PassiveMotion(int x, int y)
{
	return GLvoid();
}

GLvoid Mouse(int button, int state, int x, int y)
{
	return GLvoid();
}

GLvoid SpecialKeyboard(int key, int x, int y)
{
	return GLvoid();
}