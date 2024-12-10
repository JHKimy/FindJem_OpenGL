#pragma once

#include "stdafx.h"
#include <vector>
#include <string>
using namespace std;

class Actor
{
private:
    GLuint vao, vbo, ebo;             // OpenGL ���� ��ü
    vector<glm::vec3> vertices; // ���� ������
    glm::vec3 color;                 // ����

    void InitializeBuffers();        // ���� �ʱ�ȭ
    void LoadObj(const std::string& objFilePath); // OBJ ���� �ε�

protected:
    glm::vec3 position;              // ��ġ
    glm::vec3 scale;                 // ũ��
    glm::vec3 rotation;              // ȸ�� (yaw, pitch, roll)


public:
    Actor(string objFilePath,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 scale = glm::vec3(1.0f),
        glm::vec3 rotation = glm::vec3(0.0f),
        glm::vec3 color = glm::vec3(1.0f));
    virtual ~Actor();

    virtual void Render(GLuint shaderProgram);

    // ��ȯ ���� �Լ�
    void SetPosition(const glm::vec3& pos);
    void SetScale(const glm::vec3& scl);
    void SetRotation(const glm::vec3& rot);

    glm::vec3 GetPosition() const;
    glm::vec3 GetScale() const;
    glm::vec3 GetRotation() const;
};