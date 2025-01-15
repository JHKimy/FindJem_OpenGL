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

	// 이전 위치 저장 (충돌 감지용)
	glm::vec3 prevCharacterPosition = character->GetPosition();

	// 이동 방향 계산
	glm::vec3 moveDirection(0.0f);


	// === 클라에서 서버 보내는 패킷 ======================== 

	CS_PLAYER_PACKET p;
	p.dirY = g_yaw;

	// 나중에 변수 처리??
	// 카메라 1인칭 시점 설정
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
	// 서버에 보내기
	networkmanager.SendPlayerMove(p);

	// =========================================

	/*moveDirection += character->GetForwardVector();
	moveDirection -= character->GetForwardVector();
	moveDirection -= glm::cross(character->GetForwardVector(), glm::vec3(0.0f, 1.0f, 0.0f));
	moveDirection += glm::cross(character->GetForwardVector(), glm::vec3(0.0f, 1.0f, 0.0f));*/


	// 이동 수행 (임시로 이동)
	//if (glm::length(moveDirection) > 0.0f) {
	//	moveDirection = glm::normalize(moveDirection);
	//	character->Move(moveDirection);
	//}

	// 충돌 감지
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

	// 충돌이 감지되면 위치를 이전 위치로 되돌림
	if (collisionDetected) {
		character->SetPosition(prevCharacterPosition);
	}





	// 점프 처리
	if (Command[SpaceBar]) character->Jump();




	// 카메라 설정
	if (Command[Num1]) {
		isFirstPersonView = true; // 1인칭 모드 활성화
		//camera->FirstPersonView(character->GetPosition(), character->GetYaw(), 0.f);
	}
	else if (Command[Num2]) {
		isFirstPersonView = false; // 탑뷰 모드 활성화
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
		break;		// 앞으로 이동

	case 's': Command[S] = true;
		g_keyPressed = true;
		break;		// 뒤로 이동
	case 'a': Command[A] = true;
		g_keyPressed = true;
		break;		// 왼쪽으로 이동
	case 'd': Command[D] = true;
		g_keyPressed = true;
		break;		// 오른쪽으로 이동
	case 'r': Command[R] = true;
		g_keyPressed = true;
		networkmanager.SendReady();
		break;		// 준비 상태 
	case 32/*SpaceBar*/: Command[SpaceBar] = true;
		// g_keyPressed = true;??

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
		character->Shoot(); // 캐릭터가 총알 발사
	}

}

GLvoid Controller::PassiveMotion(int x, int y)
{
	float sensitivity = 0.3f;


	if (isFirstPersonView) { // 1인칭 모드일 때만 처리
		// 마우스 이동 변화에 따라 시점을 업데이트
		float deltaX = x - winWidth / 2;
		g_yaw = deltaX * sensitivity;

		float deltaY = y - winHeight / 2;
		g_pitch = deltaY * 2;
		// 캐릭터 회전 업데이트
		//character->Rotate(deltaX * sensitivity);

		// 카메라의 pitch 업데이트
		//camera->FirstPersonView(character->GetPosition(), character->GetYaw(), deltaY * sensitivity);

		glutWarpPointer(winWidth / 2, winHeight / 2); // 마우스를 중앙으로 되돌립니다.

	}
	glutPostRedisplay();

}

GLvoid Controller::SpecialKeyboard(int key, int x, int y)
{
	return GLvoid();
}