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

using enum CommandKey;  // CommandKey ����� �������� ��� ����

class Controller
{
private:
    static Controller* instance; // �̱��� �ν��Ͻ�
    Scene* scene;                // Scene ����

    Controller(Scene* scene) : scene(scene) {}
    ~Controller() {}

public:
	
	// Controller �ʱ�ȭ
	static void Initialize(Scene* scene) {
		if (!instance)
			instance = new Controller(scene);
	}

	// ������Ʈ �Լ�
	void Update(float deltaTime);

	// �Է� �̺�Ʈ
	GLvoid Keyboard(unsigned char key, int x, int y);
	GLvoid KeyboardUp(unsigned char key, int x, int y);
	GLvoid SpecialKeyboard(int key, int x, int y);
	GLvoid Mouse(int button, int state, int x, int y);
	

	// Controller ����
	static void Finalize() {
		delete instance;
		instance = nullptr;
	}
};

