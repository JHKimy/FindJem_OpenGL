#include "Camera.h"

Camera::Camera(
	glm::vec3 pos, glm::vec3 target, glm::vec3 up, 
	float fov, float aspectRatio, 
	float nearClip, float farClip
)
	: cameraPos(pos), cameraTarget(target), cameraUp(up),
	fov(fov), aspectRatio(aspectRatio), 
	nearClip(nearClip), farClip(farClip) {}

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
	// ��� ���� ��� ���
	glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
	glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);

	// ���̴��� ��� ����
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "viewTransform");
	GLuint projLoc = glGetUniformLocation(shaderProgram, "projectionTransform");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Camera::FirstPersonView(glm::vec3 characterPos)
{
	//isFirstPersonView = true;
	//
	glm::vec3 cameraOffset(0.0f, 0.0f, 0.0f); // ī�޶� ĳ���� �ڿ� ��ġ
	glm::vec3 lookAt(0.0f, 0.0f, 1.0f); // ī�޶� ĳ���� �ڿ� ��ġ
	cameraPos = characterPos + cameraOffset;
	cameraTarget = cameraPos + lookAt;
	//UpdatePosition(fpvPos);
	//UpdateTarget(fpvTarget);

}

void Camera::TopView()
{
	//isFirstPersonView = false;
	// ž���� ��ġ�� Ÿ�� ����
	cameraPos = glm::vec3(0.0f, 50.f, 0.0f); // ī�޶� Ÿ�� ���� �̵�
	cameraTarget = glm::vec3(11.0f, 0.f, 0.0f); // Ÿ���� �߽����� ����
	//UpdatePosition(topViewPos);
	//UpdateTarget(topViewTarget);
}
