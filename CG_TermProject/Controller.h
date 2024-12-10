#pragma once
#include "Scene.h"
#include <unordered_map>

enum CommandKey {
	W, A, S, D,
	SpaceBar,
	Num1, Num2,
	MouseLeftClick
};


std::unordered_map<CommandKey, bool> Command = {
	{CommandKey::W,				false},
	{CommandKey::A,				false},
	{CommandKey::S,				false},
	{CommandKey::D,				false},
	{CommandKey::SpaceBar,		false},
	{CommandKey::Num1,			true},
	{CommandKey::Num2,			false},
	{CommandKey::MouseLeftClick,false},
};

using enum CommandKey;  // CommandKey 멤버를 전역으로 사용 가능

class Controller
{
private:
    static Controller* instance; // 싱글톤 인스턴스
    Scene* scene;                // Scene 참조

    Controller(Scene* scene) : scene(scene) {}
    ~Controller() {}

public:
	
	// Controller 초기화
	static void Initialize(Scene* scene) {
		if (!instance)
			instance = new Controller(scene);
	}

	// 업데이트 함수
	void Update(float deltaTime);

	// 입력 이벤트
	GLvoid Keyboard(unsigned char key, int x, int y);
	GLvoid KeyboardUp(unsigned char key, int x, int y);
	GLvoid SpecialKeyboard(int key, int x, int y);
	GLvoid Mouse(int button, int state, int x, int y);
	

	// Controller 정리
	static void Finalize() {
		delete instance;
		instance = nullptr;
	}
};

