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


	float yaw;   // 좌우 회전값
	float pitch; // 위아래 회전값


	bool isFirstPersonView;  // 현재 시점 상태

	glm::vec3 forwardVector{ 0.0f, 0.0f, -1.f };	// 전방 벡터

	GLuint CameraShader;


	// 내부 함수로 뷰와 투영 행렬 계산 및 셰이더 업데이트
	void UpdateViewProjection(); 

public:
	Camera(GLuint shaderprogram);	// 카메라 위치		// 카메라 속성

	float GetPitch();
	// glm::mat4 GetViewMatrix() const;        // 뷰 행렬 계산
	// glm::mat4 GetProjectionMatrix() const;  // 투영 행렬 계산

	void UpdatePosition(glm::vec3 newPos);  // 카메라 위치 업데이트
	void UpdateTarget(glm::vec3 newTarget); // 카메라 타겟 업데이트
	// void SetAspectRatio(float ratio);       // 화면 비율 설정
	glm::vec3 GetPosition() const;			// 카메라 위치 가져오기

	//void ApplyCamera(GLuint shaderProgram); // 카메라 설정을 셰이더에 전달


	// 시점 전환
	void FirstPersonView(
		glm::vec3 characterPos,
		float characterYaw, 
		float deltaPitch);

	void TopView();
};

#endif // CAMERA_H