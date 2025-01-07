#pragma once
#include "Scene.h"
#include <unordered_map>
#include "Camera.h"

enum CommandKey {
	W, A, S, D,
	R,
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
	std::shared_ptr<Scene> scene = nullptr;                // Scene ����
	Camera* camera;
	Character* character;
	bool isFirstPersonView; // ���� ī�޶� ���
	float pitch{};

public:

	// ������ ������ �ݵ�� �����ڿ��� �ʱ�ȭ
	Controller(std::shared_ptr<Scene> scene, Camera* camera);

    ~Controller() {}


	// �Է� �̺�Ʈ
	GLvoid Keyboard(unsigned char key, int x, int y);
	GLvoid KeyboardUp(unsigned char key, int x, int y);
	GLvoid SpecialKeyboard(int key, int x, int y);
	GLvoid Mouse(int button, int state, int x, int y);
	GLvoid PassiveMotion(int x, int y);


	// ������Ʈ �Լ�
	void Update(float deltaTime);
};

