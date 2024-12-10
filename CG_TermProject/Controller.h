#pragma once
#include "Scene.h"
#include <unordered_map>

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

public:

	Controller(Scene* scene) :scene(scene) {}
    ~Controller() {}

	// 업데이트 함수
	void Update(float deltaTime);

	// 입력 이벤트
	static GLvoid Keyboard(unsigned char key, int x, int y);
	static GLvoid KeyboardUp(unsigned char key, int x, int y);
	static GLvoid SpecialKeyboard(int key, int x, int y);
	static GLvoid Mouse(int button, int state, int x, int y);
	
};

