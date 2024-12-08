#ifndef CAMERA_H  // CAMERA_H가 정의되지 않았으면
#define CAMERA_H  // CAMERA_H를 정의하고


#include "stdafx.h"




class Camera
{

private:
    glm::vec3 cameraPos;     // 카메라 위치
    glm::vec3 cameraTarget;  // 카메라가 바라보는 대상
    glm::vec3 cameraUp;      // 카메라의 위쪽 방향

	float fov;				// Field of View
	float aspectRatio;		// 화면 비율
	float nearClip;			// 가까운 클리핑 거리
	float farClip;			// 먼 클리핑 거리

	bool isFirstPersonView;  // 현재 시점 상태

public:
	Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up,	// 카메라 위치
		float fov = 45.0f, float aspectRatio = 1.0f,		
		float nearClip = 0.1f, float farClip = 100.0f);		// 카메라 속성


	glm::mat4 GetViewMatrix() const;       // 뷰 행렬 계산
	glm::mat4 GetProjectionMatrix() const; // 투영 행렬 계산

	void UpdatePosition(glm::vec3 newPos); // 카메라 위치 업데이트
	void UpdateTarget(glm::vec3 newTarget); // 카메라 타겟 업데이트
	void SetAspectRatio(float ratio);      // 화면 비율 설정
	glm::vec3 GetPosition() const;			// 카메라 위치 가져오기

	void ApplyCamera(GLuint shaderProgram); // 카메라 설정을 셰이더에 전달


	// 시점 전환
	void SwitchToFirstPerson(glm::vec3 characterPos, glm::vec3 forwardVector);
	void SwitchToThirdPerson(glm::vec3 characterPos, glm::vec3 forwardVector);
};

#endif // CAMERA_H