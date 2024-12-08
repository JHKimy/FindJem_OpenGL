#ifndef CAMERA_H  // CAMERA_H�� ���ǵ��� �ʾ�����
#define CAMERA_H  // CAMERA_H�� �����ϰ�


#include "stdafx.h"




class Camera
{

private:
    glm::vec3 cameraPos;     // ī�޶� ��ġ
    glm::vec3 cameraTarget;  // ī�޶� �ٶ󺸴� ���
    glm::vec3 cameraUp;      // ī�޶��� ���� ����

	float fov;				// Field of View
	float aspectRatio;		// ȭ�� ����
	float nearClip;			// ����� Ŭ���� �Ÿ�
	float farClip;			// �� Ŭ���� �Ÿ�

	bool isFirstPersonView;  // ���� ���� ����

public:
	Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up,	// ī�޶� ��ġ
		float fov = 45.0f, float aspectRatio = 1.0f,		
		float nearClip = 0.1f, float farClip = 100.0f);		// ī�޶� �Ӽ�


	glm::mat4 GetViewMatrix() const;       // �� ��� ���
	glm::mat4 GetProjectionMatrix() const; // ���� ��� ���

	void UpdatePosition(glm::vec3 newPos); // ī�޶� ��ġ ������Ʈ
	void UpdateTarget(glm::vec3 newTarget); // ī�޶� Ÿ�� ������Ʈ
	void SetAspectRatio(float ratio);      // ȭ�� ���� ����
	glm::vec3 GetPosition() const;			// ī�޶� ��ġ ��������

	void ApplyCamera(GLuint shaderProgram); // ī�޶� ������ ���̴��� ����


	// ���� ��ȯ
	void SwitchToFirstPerson(glm::vec3 characterPos, glm::vec3 forwardVector);
	void SwitchToThirdPerson(glm::vec3 characterPos, glm::vec3 forwardVector);
};

#endif // CAMERA_H