#ifndef Light_H  // Light_H�� ���ǵ��� �ʾ�����
#define Light_H  // Light_H�� �����ϰ�

#include "stdafx.h"


class Light
{
private:
    glm::vec3 lightPos;   // ������ ��ġ
    glm::vec3 lightColor; // ������ ����

public:
    Light(glm::vec3 pos, glm::vec3 color);

    void SetLightPosition(glm::vec3 pos); // ���� ��ġ ����
    void SetLightColor(glm::vec3 color);  // ���� ���� ����

    void ApplyLighting(unsigned int shaderProgram, glm::vec3 viewPos); // ���̴��� ���� ���� ����
};



#endif // Light_H