#pragma once

#include "Actor.h"
//#include "Bullet.h"
#include <vector>

class Character : public Actor
{
private:
    int health;
    float mass;
    float moveSpeed;
    bool isJumping;
    float jumpSpeed;
    float gravity;
    //std::vector<Bullet*> bullets;

public:
    Character(const glm::vec3& position);



    void Move(const glm::vec3& dir);
    void Rotate(glm::vec3 forwardVector);
    void Jump();
    void Update(float deltaTime);
    void Shoot();
    void TakeDamage(int amount);
    bool IsGameOver() const;
    float GetMoveSpeed();
    void SetMoveSpeed(float speed);
    //void Stop();
    //void ResetSpeed();
    //const std::vector<Bullet*>& GetBullets() const;


    //glm::vec3 GetPosition();
    //void SetDirection(const glm::vec3& dir);
    //glm::vec3 GetDirection() const;

    // 캐릭터 y축 회전값
    float GetYaw() const; // 추가된 메서드


    // 렌더링 함수 (Actor의 Render를 오버라이드)
    void Render(GLuint shaderProgram) override;

};

