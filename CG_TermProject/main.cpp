#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>


//uniform vec3 lightPos;	//--- 조명의 위치
//uniform vec3 lightColor;	//--- 조명의 색
//uniform vec3 objectColor;	//--- 객체의 색
//
//uniform vec3 viewPos;		//--- 카메라 위치




// 윈도우 사이즈
GLfloat winWidth{ 700 }, winHeight{ 700 };

// 카메라 정보
glm::vec3 cameraPos = glm::vec3(10.0f, 10.0f, 10.0f); // 카메라 위치
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); // 카메라가 바라보는 대상
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // 카메라의 위쪽 방향

// 빛 정보
glm::vec3 lightPos = glm::vec3(0.0f, 10.f, 10.f); // 빛의 위치
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f); // 빛의 색상


// 카메라 위치
float cameraX{ 10.f }, cameraY{ 40.f }, cameraZ{ 0.f };



// 카메라 설정
void SetCamera(GLuint shaderProgram) 
{

	// 뷰 변환 행렬 계산
	glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

	// 투영 변환 행렬 계산 (원근 투영)
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 700.0f / 700.0f, 0.1f, 100.0f);

	// 셰이더에 전달
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "viewTransform");
	GLuint projLoc = glGetUniformLocation(shaderProgram, "projectionTransform");
	
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

// 빛 설정
void SetLight(GLuint shaderProgram) {

	GLuint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
	glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);

	// 빛의 위치와 색상을 셰이더에 전달
	GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
	glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
	
	// 거울 반사
	GLuint viewPosLocation = glGetUniformLocation(shaderProgram, "viewPos"); //--- viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, cameraX, cameraY, cameraZ);
}


// 콜백 함수
GLvoid drawScene();
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
GLvoid Motion(int x, int y);
GLvoid TimerFunction(int value);
GLvoid PassiveMotion(int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);


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


// 큐브의 정점 위치와 색상 데이터
float cubeData[] = {
	// x, y, z          nx, ny, nz
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // 0
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // 1
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // 2
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // 3

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // 4
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // 5
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // 6
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // 7

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  // 8
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  // 9
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  // 10
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  // 11

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  // 12
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  // 13
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  // 14
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  // 15

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  // 16
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  // 17
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  // 18
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  // 19

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  // 20
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  // 21
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  // 22
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f   // 23
};

unsigned int cubeIndices[] = {
	0, 1, 2,  0, 2, 3,       // 앞면
	4, 5, 6,  4, 6, 7,       // 뒷면
	8, 9, 10, 8, 10, 11,     // 왼쪽면
	12, 13, 14, 12, 14, 15,  // 오른쪽면
	16, 17, 18, 16, 18, 19,  // 아래면
	20, 21, 22, 20, 22, 23   // 윗면
};

// 버텍스 버퍼 오브젝트
// 버텍스 정보를 GPU 메모리에 보내주기 위한 객체 
// (위치, 색상, 좌표 등등)
GLuint cubeVBO;

// 인덱스 버퍼 오브젝트
GLuint cubeEBO;

// VBO,EBO 관리 시스템
GLuint cubeVAO;



void InitBuffer() 
{

// VAO
	// 큐브 VAO 생성
	glGenVertexArrays(1, &cubeVAO);
	// 큐브 VAO 활성화
	glBindVertexArray(cubeVAO);

// VBO
	// 큐브 VBO 생성
	glGenBuffers(1, &cubeVBO);
	// 큐브 VBO 활성화
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	// VBO에 데이터 업로드
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeData), cubeData, GL_STATIC_DRAW);

// EBO
	// EBO 생성
	glGenBuffers(1, &cubeEBO);
	// EBO 활성화
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
	// EBO에 데이터 업로드
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

// 위치
	// VBO에 저장된 데이터 중 정점 위치 데이터를 셰이더와 연결
	// (속성 번호, 속성당 개수, 데이터 타입, 데이터 정규화, stride, 시작 주소)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// 0번 속성을 활성화 -> VBO가 읽게
	glEnableVertexAttribArray(0);
	
// 색상
	// VBO에 저장된 데이터 중 정점 색상 데이터
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);



// VAO
	// VAO 활성화 해제
	glBindVertexArray(0);
}

void DrawTest()
{
	// 큐브 버텍스 버퍼
	glBindVertexArray(cubeVAO);


}







void main(int argc, char** argv)
{

// ===============================================================
	//--- 윈도우 생성하기
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

	// 큐브 버퍼 초기화
	InitBuffer();

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

	// 셰이더 사용해서 렌더링
	glUseProgram(shaderProgram);

	// VAO 설정 열기
	glBindVertexArray(cubeVAO);

	// 카메라와 빛 설정
	SetCamera(shaderProgram);

	// 큐브 그리기
	GLuint cubeObjectColor = glGetUniformLocation(shaderProgram, "objectColor");
	glUniform3f(cubeObjectColor, 0.0f, 1.0f, 0.0f);

	// 기본 단위 행렬 (1.0f)
	glm::mat4 model = glm::mat4(1.0f);
	// `modelTransform` 유니폼 변수 위치 가져오기
	GLuint modelLoc = glGetUniformLocation(shaderProgram, "modelTransform");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	SetLight(shaderProgram);


	// 전면 버퍼와 후면 버퍼 스왑
	glutSwapBuffers();
	glutPostRedisplay();

}



GLvoid Keyboard(unsigned char key, int x, int y)
{
	return GLvoid();
}

GLvoid KeyboardUp(unsigned char key, int x, int y)
{
	return GLvoid();
}

GLvoid Motion(int x, int y)
{
	return GLvoid();
}

GLvoid TimerFunction(int value)
{
	return GLvoid();
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