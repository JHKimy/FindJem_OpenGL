#pragma once
#include "Scene.h"
#include <unordered_map>
#include "Camera.h"

enum CommandKey {
	W, A, S, D,
	SpaceBar,
	Num1, Num2,
	MouseLeftClick
};

// 전역 변수 선언
extern std::unordered_map<CommandKey, bool> Command;


using enum CommandKey;  // CommandKey 멤버를 전역으로 사용 가능

class Controller
{
private:
	Scene* scene = nullptr;                // Scene 참조
	Camera* camera;
	Character* character;

public:

	// 참조형 변수는 반드시 생성자에서 초기화
	Controller(Scene* scene, Camera* camera) 
		:scene(scene), camera(camera) {}
    ~Controller() {}


	// 입력 이벤트
	static GLvoid Keyboard(unsigned char key, int x, int y);
	static GLvoid KeyboardUp(unsigned char key, int x, int y);
	static GLvoid SpecialKeyboard(int key, int x, int y);
	static GLvoid Mouse(int button, int state, int x, int y);
	static GLvoid PassiveMotion(int x, int y);


	// 업데이트 함수
	void Update(float deltaTime);
};

