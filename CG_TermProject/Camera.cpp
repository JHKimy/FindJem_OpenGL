#include "Camera.h"
#include "Global.h"

Camera::Camera(GLuint shaderProgram) : shaderProgram(shaderProgram)
{
	cameraUp = glm::vec3(0.f, 1.f, 0.f);
	fov = 45.f;				// Field of View
	aspectRatio = 1.0f;		// ȭ�� ����
	nearClip = 0.01f;		// ����� Ŭ���� �Ÿ�
	farClip = 200.f;		// �� Ŭ���� �Ÿ�
}


void Camera::ApplyCamera(GLuint shaderProgram)
{

	// ��� ���� ��� ���
	glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
	
	// 3��Ī�϶�
	if (!isFirstPersonView) {
		fov = 45.f;
	}
	else {
		fov = 60.f;
	}

	glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);

	// 3��Ī�϶�
	if (!isFirstPersonView) {
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, 10.0)); //--- ������ z�� �̵�
	}


	// ���̴��� ��� ����
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "viewTransform");
	GLuint projLoc = glGetUniformLocation(shaderProgram, "projectionTransform");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Camera::FirstPersonView(glm::vec3 characterPos, float characterYaw)
{
	isFirstPersonView = true;

	glm::vec3 direction;
	direction.x = cos(glm::radians(characterYaw)); // �¿� ����
	direction.y = 0.0f;                            // ���� ���� ����
	direction.z = sin(glm::radians(characterYaw)); // ���� ����
	
	// ī�޶� ĳ���� �տ� ��ġ
	glm::vec3 cameraOffset(
		direction.x,
		direction.y + 1.f ,
		direction.z);

	cameraPos = characterPos + cameraOffset;
	cameraTarget = cameraPos + glm::normalize(direction);
	cameraUp = glm::vec3(0.f, 1.f, 0.f);

	fov = 60.f;





	// ��� ���� ��� ���
	glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

	glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);

	// ���̴��� ��� ����
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "viewTransform");
	GLuint projLoc = glGetUniformLocation(shaderProgram, "projectionTransform");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

}

void Camera::TopView()
{
	isFirstPersonView = false;
	// ž���� ��ġ�� Ÿ�� ����
	cameraPos = glm::vec3(20.f, 150.f, 0.1f); // ī�޶� Ÿ�� ���� �̵�
	cameraTarget = glm::vec3(20.f, 0.f, 20.f); // Ÿ���� �߽����� ����
	cameraUp = glm::vec3(0.f, 1.f, 0.f);

	fov = 45.f;

	// ��� ���� ��� ���
	glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
	glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);

	// ���̴��� ��� ����
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "viewTransform");
	GLuint projLoc = glGetUniformLocation(shaderProgram, "projectionTransform");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));



	//UpdatePosition(topViewPos);
	//UpdateTarget(topViewTarget);
}












void Camera::UpdatePosition(glm::vec3 newPos)
{
	cameraPos = newPos;
}

void Camera::UpdateTarget(glm::vec3 newTarget)
{
	cameraTarget = newTarget;
}

glm::vec3 Camera::GetPosition() const
{
	return cameraPos;
}