#include "Controller.h"
#include "Global.h"
#include "iostream"



std::unordered_map<CommandKey, bool> Command = {
	{CommandKey::W,				false},
	{CommandKey::A,				false},
	{CommandKey::S,				false},
	{CommandKey::D,				false},
	{CommandKey::SpaceBar,		false},
	{CommandKey::Num1,			false},
	{CommandKey::Num2,			false},
	{CommandKey::MouseLeftClick,false},
};



void Controller::Update(float deltaTime)
{
	character = scene->GetCharacter();

	// 이전 위치 저장 (충돌 처리)
	glm::vec3 prevCharacterPosition = character->GetPosition();


	// 회전
	character->Rotate(forwardVector);



	// 충돌 감지
	bool collisionDetected = false;
	for (Actor* otherActor : scene->GetActors()) {
		if (character != otherActor && 
			character->CheckCollision(otherActor))	
		{
			collisionDetected = true;
			//std::cout << "Collision with Actor ID: " << otherActor->GetPosition().x << std::endl;
			break;
		}
	}


	//prevCharacterPosition = character->GetPosition();

	//if (!collisionDetected)
	//{
		if (Command[W]) character->Move(forwardVector);
		if (Command[S]) character->Move(-forwardVector);
		if (Command[A]) character->Move(-glm::cross(forwardVector, glm::vec3(0.0f, 1.0f, 0.0f)));
		if (Command[D]) character->Move(glm::cross(forwardVector, glm::vec3(0.0f, 1.0f, 0.0f)));
		if (Command[SpaceBar]) character->Jump();
	//}

	else if (collisionDetected) {
		character->SetPosition(prevCharacterPosition - (forwardVector)*0.005f);
	}

	prevCharacterPosition = character->GetPosition();


	// 1인칭 시점
	if (Command[Num1])
	{
		camera->FirstPersonView(
			character->GetPosition(),
			rotateY);
	}
	// 탑 뷰 시점
	if (Command[Num2]) {

		camera->TopView();
	}



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

	case 32/*SpaceBar*/: Command[SpaceBar] = true; 
		
		break; // 캐릭터 점프	

		 

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
	return GLvoid();
}

GLvoid Controller::PassiveMotion(int x, int y)
{
	float sensitivity = 0.1f;  // 마우스 민감도

	// 마우스 이동 변화에 따라 시점을 업데이트
	float deltaX = x - winWidth / 2;
	float deltaY = y - winHeight / 2;

	// 시점 변화에 따라 캐릭터의 전방 방향 업데이트
	rotateY += sensitivity * deltaX;
	if (rotateY > 360.0f) rotateY -= 360.0f;
	if (rotateY < 0.0f) rotateY += 360.0f;

	float pitch = sensitivity * deltaY;
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	forwardVector.x = cos(glm::radians(rotateY)) * cos(glm::radians(pitch));
	forwardVector.y = sin(glm::radians(pitch));
	forwardVector.z = sin(glm::radians(rotateY)) * cos(glm::radians(pitch));

	glutWarpPointer(winWidth / 2, winHeight / 2); // 마우스를 중앙으로 되돌립니다.
	glutPostRedisplay();
}

GLvoid Controller::SpecialKeyboard(int key, int x, int y)
{
	return GLvoid();
}