#include "Camera.h"
#include "Global.h"

Camera::Camera(
	glm::vec3 pos, glm::vec3 target, glm::vec3 up
)
	: cameraPos(pos), cameraTarget(target), cameraUp(up) 
{

	fov = 45.f;
	aspectRatio = 1.0f;
	nearClip = 0.01f;
	farClip = 200.f;

}

//glm::mat4 Camera::GetViewMatrix() const
//{
//	return glm::lookAt(cameraPos, cameraTarget, cameraUp);
//}
//
//
//glm::mat4 Camera::GetProjectionMatrix() const
//{
//	return glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
//}

void Camera::UpdatePosition(glm::vec3 newPos)
{
	cameraPos = newPos;
}

void Camera::UpdateTarget(glm::vec3 newTarget)
{
	cameraTarget = newTarget;
}

//void Camera::SetAspectRatio(float ratio)
//{
//	aspectRatio = ratio;
//}

glm::vec3 Camera::GetPosition() const
{
	return cameraPos;
}

void Camera::ApplyCamera(GLuint shaderProgram)
{

	// 뷰와 투영 행렬 계산
	glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
	
	// 3인칭일때
	if (!isFirstPersonView) {
		fov = 45.f;
	}
	else {
		fov = 60.f;
	}

	glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);

	// 3인칭일때
	if (!isFirstPersonView) {
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, 10.0)); //--- 공간을 z축 이동
	}


	// 셰이더에 행렬 전달
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "viewTransform");
	GLuint projLoc = glGetUniformLocation(shaderProgram, "projectionTransform");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Camera::FirstPersonView(glm::vec3 characterPos, float characterYaw)
{
	isFirstPersonView = true;

	glm::vec3 direction;
	direction.x = cos(glm::radians(characterYaw)); // 좌우 방향
	direction.y = 0.0f;                            // 상하 방향 없음
	direction.z = sin(glm::radians(characterYaw)); // 전후 방향
	
	// 카메라가 캐릭터 앞에 위치
	glm::vec3 cameraOffset(
		direction.x,
		direction.y + 1.f ,
		direction.z);

	cameraPos = characterPos + cameraOffset;
	cameraTarget = cameraPos + glm::normalize(direction);

	//// 뷰와 투영 행렬 계산
	//glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
	//glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
	////projection = glm::translate(projection, glm::vec3(0.0, 0.0, 0.0)); //--- 공간을 z축 이동

	//// 셰이더에 행렬 전달
	//GLuint viewLoc = glGetUniformLocation(shaderProgram, "viewTransform");
	//GLuint projLoc = glGetUniformLocation(shaderProgram, "projectionTransform");
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	////UpdatePosition(fpvPos);
	////UpdateTarget(fpvTarget);

}

void Camera::TopView()
{
	isFirstPersonView = false;
	// 탑뷰의 위치와 타겟 설정
	cameraPos = glm::vec3(20.f, 150.f, 0.1f); // 카메라를 타겟 위로 이동
	cameraTarget = glm::vec3(20.f, 0.f, 20.f); // 타겟을 중심으로 설정
	//UpdatePosition(topViewPos);
	//UpdateTarget(topViewTarget);
}
