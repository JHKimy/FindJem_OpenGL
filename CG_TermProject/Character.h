#pragma once

#include "Actor.h"
//#include "Bullet.h"
#include <vector>
#include <memory> // 스마트 포인터 사용

class Bullet;

class Character : public Actor
{
private:
    
    glm::vec3 forwardVector2;
    float yaw;

    int health;
    float mass;
    float moveSpeed;
    bool isJumping;
    float jumpSpeed;
    float gravity;
    //std::vector<Bullet*> bullets;


    // 총알 리스트
    std::vector<std::unique_ptr<Bullet>> bullets; 

public:
    Character(const glm::vec3& position);

    glm::vec3 GetForwardVector() const;

    void Move(const glm::vec3& dir);
    void Rotate(float deltaYaw);
    void Jump();
    void Update(float deltaTime);
    void Shoot();
    void TakeDamage(int amount);
    bool IsGameOver() const;
    float GetMoveSpeed();
    void SetMoveSpeed(float speed);


    void UpdateBullets(float deltaTime); // 총알 업데이트
    // 총알 리스트 반환
    const std::vector<std::unique_ptr<Bullet>>& GetBullets() const; 
    
    
    //void Stop();
    //void ResetSpeed();
    //const std::vector<Bullet*>& GetBullets() const;


    //glm::vec3 GetPosition();
    //void SetDirection(const glm::vec3& dir);
    //glm::vec3 GetDirection() const;

    // 캐릭터 y축 회전값
    float GetYaw() const;


    // 렌더링 함수 (Actor의 Render를 오버라이드)
    void Render(GLuint shaderProgram) override;

};

