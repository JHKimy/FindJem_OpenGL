#version 330 core
 // 응용 프로그램에서 받아온 도형 좌표값
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;

// 프래그먼트 셰이더 연동 변수
out vec3 FragPos; // 객체의 위치값
out vec3 Normal; // 노멀값

// 버텍스 변환 행렬 (uniform)
uniform mat4 modelTransform; // 모델링 변환 행렬: uniform 변수로 선언
uniform mat4 projectionTransform;
uniform mat4 viewTransform;

void main()
{
   gl_Position = projectionTransform * viewTransform * modelTransform * vec4(vPos, 1.0); //--- 좌표값에 modelTransform 변환을 적용한다.
   
   
   FragPos = vec3(modelTransform * vec4(vPos, 1.0));
   Normal = vNormal;
}