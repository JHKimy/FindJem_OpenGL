#ifndef Light_H  // Light_H가 정의되지 않았으면
#define Light_H  // Light_H를 정의하고

#include "stdafx.h"


class Light
{
private:
    glm::vec3 lightPos;   // 조명의 위치
    glm::vec3 lightColor; // 조명의 색상

public:
    Light(glm::vec3 pos, glm::vec3 color);

    void SetLightPosition(glm::vec3 pos); // 조명 위치 설정
    void SetLightColor(glm::vec3 color);  // 조명 색상 설정

    void ApplyLighting(unsigned int shaderProgram, glm::vec3 viewPos); // 셰이더에 조명 정보 전달
};



#endif // Light_H