#pragma once
#include "Scene.h"
#include <unordered_map>

enum CommandKey {
	W, A, S, D,
	SpaceBar,
	Num1, Num2,
	MouseLeftClick
};

// ���� ���� ����
extern std::unordered_map<CommandKey, bool> Command;


using enum CommandKey;  // CommandKey ����� �������� ��� ����

class Controller
{
private:
	Scene* scene = nullptr;                // Scene ����

public:

	Controller(Scene* scene) :scene(scene) {}
    ~Controller() {}

	// ������Ʈ �Լ�
	void Update(float deltaTime);

	// �Է� �̺�Ʈ
	static GLvoid Keyboard(unsigned char key, int x, int y);
	static GLvoid KeyboardUp(unsigned char key, int x, int y);
	static GLvoid SpecialKeyboard(int key, int x, int y);
	static GLvoid Mouse(int button, int state, int x, int y);
	
};

