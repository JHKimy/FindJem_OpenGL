#include "Light.h"

Light::Light(glm::vec3 pos, glm::vec3 color)
	: lightPos(pos), lightColor(color) {}

void Light::SetLightPosition(glm::vec3 pos)
{
	lightPos = pos;
}

void Light::SetLightColor(glm::vec3 color)
{
	lightColor = color;
}

void Light::ApplyLighting(unsigned int shaderProgram, glm::vec3 viewPos)
{ 
    // ������ ��ġ�� ������ ���̴��� ����
    GLint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    GLint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");

    glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);

    // ī�޶�(������) ��ġ ���� (�ſ� �ݻ��)
    GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
    glUniform3f(viewPosLoc, viewPos.x, viewPos.y, viewPos.z);
}
