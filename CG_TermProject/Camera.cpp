#include "Camera.h"

Camera::Camera(
	glm::vec3 pos, glm::vec3 target, glm::vec3 up, 
	float fov, float aspectRatio, 
	float nearClip, float farClip
)
	: cameraPos(pos), cameraTarget(target), cameraUp(up),
	fov(fov), aspectRatio(aspectRatio), 
	nearClip(nearClip), farClip(farClip) {}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(cameraPos, cameraTarget, cameraUp);
}


glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);;
}

void Camera::UpdatePosition(glm::vec3 newPos)
{
	cameraPos = newPos;
}

void Camera::UpdateTarget(glm::vec3 newTarget)
{
	cameraTarget = newTarget;
}

void Camera::SetAspectRatio(float ratio)
{
	aspectRatio = ratio;
}

glm::vec3 Camera::GetPosition() const
{
	return cameraPos;
}

void Camera::ApplyCamera(GLuint shaderProgram)
{
	// ��� ���� ��� ���
	glm::mat4 view = GetViewMatrix();
	glm::mat4 projection = GetProjectionMatrix();

	// ���̴��� ��� ����
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "viewTransform");
	GLuint projLoc = glGetUniformLocation(shaderProgram, "projectionTransform");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Camera::SwitchToFirstPerson(glm::vec3 characterPos, glm::vec3 forwardVector)
{
	isFirstPersonView = true;
	glm::vec3 fpvPos = characterPos + glm::vec3(0.0f, 5.f, 5.0f);
	glm::vec3 fpvTarget = fpvPos + forwardVector;
	UpdatePosition(fpvPos);
	UpdateTarget(fpvTarget);
}

void Camera::SwitchToTopView(glm::vec3 characterPos)
{
	isFirstPersonView = false;
	// ž���� ��ġ�� Ÿ�� ����
	glm::vec3 topViewPos = glm::vec3(0.0f, 50.f, 0.0f); // ī�޶� Ÿ�� ���� �̵�
	glm::vec3 topViewTarget = characterPos; // Ÿ���� �߽����� ����

	UpdatePosition(topViewPos);
	UpdateTarget(topViewTarget);
}
