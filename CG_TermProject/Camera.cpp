#include "Camera.h"
#include "Global.h"



Camera::Camera(GLuint shaderProgram)
	: CameraShader(shaderProgram)
{
	cameraUp = glm::vec3(0.f, 1.f, 0.f);
	fov = 45.f;				// Field of View
	aspectRatio = 1.0f;		// 화면 비율
	nearClip = 0.01f;		// 가까운 클리핑 거리
	farClip = 200.f;		// 먼 클리핑 거리
}

float Camera::GetPitch()
{
	return pitch;
}

void Camera::UpdateViewProjection()
{
	glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
	glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);

	// 셰이더에 행렬 전달
	GLuint viewLoc = glGetUniformLocation(CameraShader, "viewTransform");
	GLuint projLoc = glGetUniformLocation(CameraShader, "projectionTransform");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}
//void Camera::ApplyCamera(GLuint shaderProgram)
//{
//
//	// 뷰와 투영 행렬 계산
//	glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
//	
//	// 3인칭일때
//	if (!isFirstPersonView) {
//		fov = 45.f;
//	}
//	else {
//		fov = 60.f;
//	}
//
//	glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
//
//	// 3인칭일때
//	if (!isFirstPersonView) {
//		projection = glm::translate(projection, glm::vec3(0.0, 0.0, 10.0)); //--- 공간을 z축 이동
//	}
//
//
//	// 셰이더에 행렬 전달
//	GLuint viewLoc = glGetUniformLocation(shaderProgram, "viewTransform");
//	GLuint projLoc = glGetUniformLocation(shaderProgram, "projectionTransform");
//	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
//	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
//}

void Camera::FirstPersonView(
	glm::vec3 characterPos, 
	float characterYaw, 
	float deltaPitch)
{
	//isFirstPersonView = true;

	yaw = characterYaw;
	pitch += deltaPitch;

	// Pitch 제한 (±60도)
	if (pitch > 20.0f) pitch = 20.0f;
	if (pitch < -20.0f) pitch = -20.0f;


	// 방향 벡터 계산
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(-pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction = glm::normalize(direction);

	// 카메라 위치 설정: 캐릭터의 전방 방향으로 오프셋 적용
	glm::vec3 cameraOffset = direction * 1.00f; // 캐릭터 앞쪽 2.0 단위 거리
	cameraOffset.y += 1.5f; // 캐릭터 머리 위로 약간 이동
	cameraPos = characterPos + cameraOffset;


	cameraTarget = cameraPos + direction;
	cameraUp = glm::vec3(0.f, 1.f, 0.f);

	fov = 60.f;



	// 뷰와 투영 행렬 업데이트
	UpdateViewProjection();

}

void Camera::TopView()
{
	//isFirstPersonView = false;
	// 탑뷰의 위치와 타겟 설정
	cameraPos = glm::vec3(20.f, 150.f, 0.1f); // 카메라를 타겟 위로 이동
	cameraTarget = glm::vec3(20.f, 0.f, 20.f); // 타겟을 중심으로 설정
	cameraUp = glm::vec3(0.f, 1.f, 0.f);

	fov = 45.f;

	// 뷰와 투영 행렬 업데이트
	UpdateViewProjection();
}












void Camera::UpdatePosition(glm::vec3 newPos)
{
	cameraPos = newPos;
	UpdateViewProjection();

}

void Camera::UpdateTarget(glm::vec3 newTarget)
{
	cameraTarget = newTarget;
	UpdateViewProjection();

}

glm::vec3 Camera::GetPosition() const
{
	return cameraPos;
}