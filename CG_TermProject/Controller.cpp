#include "Controller.h"
#include "Global.h"
#include "iostream"

#include "Character.h"
#include "Scene.h"
#include "../FindJem_Server/FindJem_Server/protocol.h"



std::unordered_map<CommandKey, bool> Command = {
	{CommandKey::W,				false},
	{CommandKey::A,				false},
	{CommandKey::S,				false},
	{CommandKey::D,				false},
	{CommandKey::R,				false},
	{CommandKey::SpaceBar,		false},
	{CommandKey::Num1,			false},
	{CommandKey::Num2,			false},
	{CommandKey::MouseLeftClick,false}
};


Controller::Controller
(std::shared_ptr<Scene> scene, Camera* camera)
	: scene(scene), camera(camera)
{
}


void Controller::Update(float deltaTime)
{
	character = scene->GetCharacter();

	// ���� ��ġ ���� (�浹 ������)
	glm::vec3 prevCharacterPosition = character->GetPosition();

	// �̵� ���� ���
	glm::vec3 moveDirection(0.0f);


	// === Ŭ�󿡼� ���� ������ ��Ŷ ======================== 

	CS_PLAYER_PACKET p;
	p.dirY = g_yaw;

	// ���߿� ���� ó��??
	// ī�޶� 1��Ī ���� ����
	camera->FirstPersonView(
		character->GetPosition(),
		character->GetYaw(),
		g_pitch * 0.1f);

	if (g_keyPressed)
	{
		p.player_id = g_id;
		if (Command[W]) {
			p.direction = 0;
		}
		if (Command[S]) {
			p.direction = 1;
		}
		if (Command[A]) {
			p.direction = 2;
		}
		if (Command[D]) {
			p.direction = 3;
		}

	}
	// ������ ������
	networkmanager.SendPlayerMove(p);

	// =========================================

	/*moveDirection += character->GetForwardVector();
	moveDirection -= character->GetForwardVector();
	moveDirection -= glm::cross(character->GetForwardVector(), glm::vec3(0.0f, 1.0f, 0.0f));
	moveDirection += glm::cross(character->GetForwardVector(), glm::vec3(0.0f, 1.0f, 0.0f));*/


	// �̵� ���� (�ӽ÷� �̵�)
	//if (glm::length(moveDirection) > 0.0f) {
	//	moveDirection = glm::normalize(moveDirection);
	//	character->Move(moveDirection);
	//}

	// �浹 ����
	bool collisionDetected = false;
	for (const auto& otherActor : scene->GetActors())
	{
		if (character != otherActor.get()
			&& character->CheckCollision(otherActor.get()))
		{
			collisionDetected = true;
			break;
		}
	}

	// �浹�� �����Ǹ� ��ġ�� ���� ��ġ�� �ǵ���
	if (collisionDetected) {
		character->SetPosition(prevCharacterPosition);
	}





	// ���� ó��
	if (Command[SpaceBar]) character->Jump();




	// ī�޶� ����
	if (Command[Num1]) {
		isFirstPersonView = true; // 1��Ī ��� Ȱ��ȭ
		//camera->FirstPersonView(character->GetPosition(), character->GetYaw(), 0.f);
	}
	else if (Command[Num2]) {
		isFirstPersonView = false; // ž�� ��� Ȱ��ȭ
		const auto& mazeMap = scene->GetMazeMap();

		camera->TopView();
	}
}



GLvoid Controller::Keyboard(unsigned char key, int x, int y)
{
	//glm::vec3 moveDir(0.0f);

	switch (key)
	{
	case 'w': Command[W] = true;
		g_keyPressed = true;
		break;		// ������ �̵�

	case 's': Command[S] = true;
		g_keyPressed = true;
		break;		// �ڷ� �̵�
	case 'a': Command[A] = true;
		g_keyPressed = true;
		break;		// �������� �̵�
	case 'd': Command[D] = true;
		g_keyPressed = true;
		break;		// ���������� �̵�
	case 'r': Command[R] = true;
		g_keyPressed = true;
		networkmanager.SendReady();
		break;		// �غ� ���� 
	case 32/*SpaceBar*/: Command[SpaceBar] = true;
		// g_keyPressed = true;??

		break; // ĳ���� ����	



	case '1':
		Command[Num1] = true;
		Command[Num2] = false;
		glutSetCursor(GLUT_CURSOR_NONE);

		break;
	case '2':
		Command[Num2] = true;
		Command[Num1] = false;
		glutSetCursor(GLUT_CURSOR_INHERIT);

		break;

	case 27:
		glutLeaveMainLoop();
	}

	// ���� ����
	// character->SetDirection(moveDir);

	glutPostRedisplay();
}

GLvoid Controller::KeyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':Command[W] = false;
		g_keyPressed = false;

		break;
	case 's':Command[S] = false;
		g_keyPressed = false;
		break;
	case 'a':Command[A] = false;
		g_keyPressed = false;
		break;
	case 'd':Command[D] = false;
		g_keyPressed = false;
		break;

	case 32: Command[SpaceBar] = false; return;

	case '1': /*Command[Num1] = false;*/ break;
	case '2': /*Command[Num2] = false;*/ break;

	case 'q':
		glutLeaveMainLoop();
		break;
	}

	glutPostRedisplay();
}

GLvoid Controller::Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		character->Shoot(); // ĳ���Ͱ� �Ѿ� �߻�
	}

}

GLvoid Controller::PassiveMotion(int x, int y)
{
	float sensitivity = 0.3f;


	if (isFirstPersonView) { // 1��Ī ����� ���� ó��
		// ���콺 �̵� ��ȭ�� ���� ������ ������Ʈ
		float deltaX = x - winWidth / 2;
		g_yaw = deltaX * sensitivity;

		float deltaY = y - winHeight / 2;
		g_pitch = deltaY * 2;
		// ĳ���� ȸ�� ������Ʈ
		//character->Rotate(deltaX * sensitivity);

		// ī�޶��� pitch ������Ʈ
		//camera->FirstPersonView(character->GetPosition(), character->GetYaw(), deltaY * sensitivity);

		glutWarpPointer(winWidth / 2, winHeight / 2); // ���콺�� �߾����� �ǵ����ϴ�.

	}
	glutPostRedisplay();

}

GLvoid Controller::SpecialKeyboard(int key, int x, int y)
{
	return GLvoid();
}