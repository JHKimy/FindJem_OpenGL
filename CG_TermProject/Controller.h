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

// ���� ���� ����
extern std::unordered_map<CommandKey, bool> Command;


using enum CommandKey;  // CommandKey ����� �������� ��� ����

class Controller
{
private:
	Scene* scene = nullptr;                // Scene ����
	Camera* camera;
	Character* character;

public:

	// ������ ������ �ݵ�� �����ڿ��� �ʱ�ȭ
	Controller(Scene* scene, Camera* camera) 
		:scene(scene), camera(camera) {}
    ~Controller() {}


	// �Է� �̺�Ʈ
	static GLvoid Keyboard(unsigned char key, int x, int y);
	static GLvoid KeyboardUp(unsigned char key, int x, int y);
	static GLvoid SpecialKeyboard(int key, int x, int y);
	static GLvoid Mouse(int button, int state, int x, int y);
	static GLvoid PassiveMotion(int x, int y);


	// ������Ʈ �Լ�
	void Update(float deltaTime);
};

