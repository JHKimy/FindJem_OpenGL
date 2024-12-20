#include "Camera.h"
#include "Global.h"



Camera::Camera(GLuint shaderProgram)
	: CameraShader(shaderProgram)
{
	cameraUp = glm::vec3(0.f, 1.f, 0.f);
	fov = 45.f;				// Field of View
	aspectRatio = 1.0f;		// ȭ�� ����
	nearClip = 0.01f;		// ����� Ŭ���� �Ÿ�
	farClip = 200.f;		// �� Ŭ���� �Ÿ�
}

float Camera::GetPitch()
{
	return pitch;
}

void Camera::UpdateViewProjection()
{
	glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
	glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);

	// ���̴��� ��� ����
	GLuint viewLoc = glGetUniformLocation(CameraShader, "viewTransform");
	GLuint projLoc = glGetUniformLocation(CameraShader, "projectionTransform");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}
//void Camera::ApplyCamera(GLuint shaderProgram)
//{
//
//	// ��� ���� ��� ���
//	glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
//	
//	// 3��Ī�϶�
//	if (!isFirstPersonView) {
//		fov = 45.f;
//	}
//	else {
//		fov = 60.f;
//	}
//
//	glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
//
//	// 3��Ī�϶�
//	if (!isFirstPersonView) {
//		projection = glm::translate(projection, glm::vec3(0.0, 0.0, 10.0)); //--- ������ z�� �̵�
//	}
//
//
//	// ���̴��� ��� ����
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

	// Pitch ���� (��60��)
	if (pitch > 20.0f) pitch = 20.0f;
	if (pitch < -20.0f) pitch = -20.0f;


	// ���� ���� ���
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(-pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction = glm::normalize(direction);

	// ī�޶� ��ġ ����: ĳ������ ���� �������� ������ ����
	glm::vec3 cameraOffset = direction * 1.00f; // ĳ���� ���� 2.0 ���� �Ÿ�
	cameraOffset.y += 1.5f; // ĳ���� �Ӹ� ���� �ణ �̵�
	cameraPos = characterPos + cameraOffset;


	cameraTarget = cameraPos + direction;
	cameraUp = glm::vec3(0.f, 1.f, 0.f);

	fov = 60.f;



	// ��� ���� ��� ������Ʈ
	UpdateViewProjection();

}

void Camera::TopView()
{
	//isFirstPersonView = false;
	// ž���� ��ġ�� Ÿ�� ����
	cameraPos = glm::vec3(20.f, 150.f, 0.1f); // ī�޶� Ÿ�� ���� �̵�
	cameraTarget = glm::vec3(20.f, 0.f, 20.f); // Ÿ���� �߽����� ����
	cameraUp = glm::vec3(0.f, 1.f, 0.f);

	fov = 45.f;

	// ��� ���� ��� ������Ʈ
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