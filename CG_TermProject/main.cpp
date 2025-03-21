﻿#include "stdafx.h"
#include "Global.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Camera.h"
#include "Light.h"
#include "Actor.h"
#include "Bullet.h"
#include "Character.h"
#include "Scene.h"
#include "Controller.h"
#include <unordered_map>
#include <memory>
#include "NetworkManager.h"
#include <chrono> // 시간 측정을 위한 헤더

using namespace std;

// 콜백 함수
GLvoid drawScene();
GLvoid TimerFunction(int value);

// 셰이더
GLuint vertexShader;
GLuint fragmentShader;

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


std::shared_ptr<Scene> mainScene = nullptr;
std::unique_ptr<Controller> mainController = nullptr;



void main(int argc, char** argv)
{
// ========================
	// 1. 서버와 연결
	if (!networkmanager.Connect())
	{
		cout << "Failed to connect to the server" << endl;
		return;
	}
// ========================

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
	glEnable(GL_DEPTH_TEST);
	//glShadeModel(GL_SMOOTH);
	//glEnable(GL_DITHER);        // 표면을 매끄럽게

// ===============================================================

	// 셰이더 프로그램 생성
	shaderProgram = make_shaderProgram();

	// 씬 생성
	mainScene = make_shared<Scene>(shaderProgram);
	
	// NetworkManager에 Scene 연결
	networkmanager.SetScene(mainScene);


	// 2. 서버로부터 데이터 수신
	networkmanager.RecvMazeData();

	// 1. 서버에 Map 수신 완료 패킷 보냄
	{
		CS_MAP_OK_PACKET p;
		p.packet_size = sizeof(p);
		p.packet_type = CS_MAP_OK;
		p.player_id = g_id;

		int retval = send(networkmanager.GetSocket(),
			reinterpret_cast<const char*>(&p), sizeof(p), 0);
	}
	// 2. 적 정보 받기
	networkmanager.RecvEnemiesData();





	mainScene->Initialize();


	// 1. Connect
	//      ↓
	// 2. 데이터 수신 스레드 시작
	// 별도 스레드 생성
	thread networkThread(
		[ & ] ( )
		{
			while (true)
			{
				networkmanager.RecvThread();
			}
		});
	networkThread.detach();


	// 컨트롤러 생성
	mainController = make_unique<Controller>(mainScene, mainScene->GetCamera());



	// 깊이 테스트 활성화
	glEnable(GL_DEPTH_TEST);



//=========================
	glutDisplayFunc(drawScene);
	// 일반 함수는 특정 객체에 의존하지 않고 독립적으로 호출
	// glutKeyboardFunc가 원하는건 독립적인 함수
	// 일반 함수는 하나의 고유한 메모리 주소 가짐
	// 
	// 멤버 함수는 객체를 통해 호출
	// 내부적으로 this 포인터를 사용
	// this 포인터는 객체의 인스턴스 주소를 가리킴
	// 멤버 함수는 호출 시 마다 해당 객체의 주소까지 필요
	// void Controller::SpecialKeyboard(Controller* this, int key, int x, int y)

	glutKeyboardFunc([](unsigned char key, int x, int y) {
		mainController->Keyboard(key, x, y); });

	glutKeyboardUpFunc([](unsigned char key, int x, int y) {
		mainController->KeyboardUp(key, x, y);});

	glutMouseFunc([](int button, int state, int x, int y) {
		mainController->Mouse(button, state, x, y);});

	glutSpecialFunc([](int key, int x, int y) {
		mainController->SpecialKeyboard(key, x, y);});

	glutPassiveMotionFunc([](int x, int y) {
		mainController->PassiveMotion(x, y); });


	glutTimerFunc(17, TimerFunction, 1);
	// glutMotionFunc(Motion);


	glutMainLoop();
//=========================

	// delete mainScene;
	// delete mainController;
}







GLvoid drawScene()
{
	// 백 버퍼에 컬러 설정
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// 컬러 버퍼와 깊이 버퍼 초기화
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//============================================================

	mainScene->Render();



	// 전면 버퍼와 후면 버퍼 스왑
	glutSwapBuffers();
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	float deltaTime = 0.016f; // 약 60FPS 기준

	mainController->Update(deltaTime);
	mainScene->Update(deltaTime);

	// 다음 프레임 요청
	glutTimerFunc(16, TimerFunction, 1);
	glutPostRedisplay();




	// FPS
	static auto lastTime = std::chrono::high_resolution_clock::now(); // 이전 프레임 시간
	static int frameCount = 0;                                       // 프레임 수
	static float elapsedTime = 0.0f;                                 // 경과 시간

	// 현재 시간 계산
	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaTime2 = currentTime - lastTime;
	lastTime = currentTime;

	// FPS 계산
	frameCount++;
	elapsedTime += deltaTime2.count();

	if (elapsedTime >= 1.0f) {
		float fps = frameCount / elapsedTime;
		//std::cout << "FPS: " << fps << std::endl;

		// 초기화
		frameCount = 0;
		elapsedTime = 0.0f;
	}
}