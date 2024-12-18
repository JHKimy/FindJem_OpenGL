#include "stdafx.h"
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



Scene* mainScene = nullptr;
Controller* mainController = nullptr;



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



	// 씬 생성
	mainScene = new Scene(shaderProgram);
	mainScene->Initialize();


	// 컨트롤러 생성
	mainController = new Controller(mainScene, mainScene->GetCamera());



	// 깊이 테스트 활성화
	glEnable(GL_DEPTH_TEST);



//=========================
	glutDisplayFunc(drawScene);
	
	glutKeyboardFunc(Controller::Keyboard);
	glutKeyboardUpFunc(Controller::KeyboardUp);
	glutMouseFunc(Controller::Mouse);
	glutSpecialFunc(Controller::SpecialKeyboard);

	glutTimerFunc(17, TimerFunction, 1);
	// glutMotionFunc(Motion);
	glutPassiveMotionFunc(Controller::PassiveMotion);

	glutMainLoop();
//=========================

	delete mainScene;
	delete mainController;
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
}

// GLvoid Motion(int x, int y)
//{
//	return GLvoid();
//}