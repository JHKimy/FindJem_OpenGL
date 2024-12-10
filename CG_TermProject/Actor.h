#pragma once

#include "stdafx.h"
#include <vector>
#include <string>
using namespace std;

class Actor
{
private:
    GLuint vao, vbo, ebo;             // OpenGL 버퍼 객체
    vector<glm::vec3> vertices; // 정점 데이터
    glm::vec3 color;                 // 색상

    void InitializeBuffers();        // 버퍼 초기화
    void LoadObj(const std::string& objFilePath); // OBJ 파일 로드

protected:
    glm::vec3 position;              // 위치
    glm::vec3 scale;                 // 크기
    glm::vec3 rotation;              // 회전 (yaw, pitch, roll)


public:
    Actor(string objFilePath,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 scale = glm::vec3(1.0f),
        glm::vec3 rotation = glm::vec3(0.0f),
        glm::vec3 color = glm::vec3(1.0f));
    virtual ~Actor();

    virtual void Render(GLuint shaderProgram);

    // 변환 관련 함수
    void SetPosition(const glm::vec3& pos);
    void SetScale(const glm::vec3& scl);
    void SetRotation(const glm::vec3& rot);

    glm::vec3 GetPosition() const;
    glm::vec3 GetScale() const;
    glm::vec3 GetRotation() const;
};