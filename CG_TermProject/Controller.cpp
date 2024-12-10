#include "Controller.h"

void Controller::Update(float deltaTime)
{
	Character* character = scene->GetCharacter();
	if (Command[W]) character->Move(glm::vec3(0, 0, 1));
	if (Command[S]) character->Move(glm::vec3(0, 0, -1));
	if (Command[A]) character->Move(glm::vec3(1, 0, 0));
	if (Command[D]) character->Move(glm::vec3(-1, 0, 0));
}

GLvoid Controller::Keyboard(unsigned char key, int x, int y)
{
	//glm::vec3 moveDir(0.0f);

	switch (key)
	{
	case 'w': Command[W] = true; break;		// 앞으로 이동
	case 's': Command[S] = true; break;		// 뒤로 이동
	case 'a': Command[A] = true; break;		// 왼쪽으로 이동
	case 'd': Command[D] = true; break;		// 오른쪽으로 이동


	case 32/*SpaceBar*/: Command[SpaceBar] = true; break; // 캐릭터 점프	



	case '1': Command[Num1] = true; break;
	case '2': Command[Num2] = true; break;
	}

	// 방향 설정
	// character->SetDirection(moveDir);

	glutPostRedisplay();
}

GLvoid Controller::KeyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':Command[W] = false; break;
	case 's':Command[S] = false; break;
	case 'a':Command[A] = false; break;
	case 'd':Command[D] = false; break;

	case 32: Command[SpaceBar] = false; return;

	case '1': Command[Num1] = false; break;
	case '2': Command[Num2] = false; break;

	case 'q':
		glutLeaveMainLoop();
		break;
	}

	glutPostRedisplay();
}

GLvoid Controller::Mouse(int button, int state, int x, int y)
{
	return GLvoid();
}

GLvoid Controller::SpecialKeyboard(int key, int x, int y)
{
	return GLvoid();
}