#pragma once

#include "Actor.h"
//#include "Bullet.h"
#include <vector>

class Character : public Actor
{
private:
    float moveSpeed;
    glm::vec3 direction;
    int health;
    bool isJumping;
    float jumpSpeed;
    float gravity;
    float mass;
    //std::vector<Bullet*> bullets;

public:
    Character(
        const std::string& objFilePath,
        const glm::vec3& position,
        const glm::vec3& scale,
        const glm::vec3& rotation, 
        const glm::vec3& color, 
        float speed = 1.0f, 
        int health = 100);



    void Move(const glm::vec3& dir);
    void Rotate(glm::vec3 forwardVector);
    void Jump();
    void Update(float deltaTime);
    void Shoot();
    void TakeDamage(int amount);
    bool IsGameOver() const;

    //const std::vector<Bullet*>& GetBullets() const;


    //glm::vec3 GetPosition();
    void SetDirection(const glm::vec3& dir);
    glm::vec3 GetDirection() const;

    // ĳ���� y�� ȸ����
    float GetYaw() const; // �߰��� �޼���










    // ������ �Լ� (Actor�� Render�� �������̵�)
    void Render(GLuint shaderProgram) override;
};

