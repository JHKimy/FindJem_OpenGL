#pragma once

#include "Actor.h"
#include "Bullet.h"
#include <vector>
#include <memory> // 스마트 포인터 사용

#include <iostream>

class Bullet;
class BulletPool;

class Character : public Actor
{
private:
    glm::vec3 forwardVector;
    float yaw;

    int health;
    float mass;
    float moveSpeed;
    bool isJumping;
    float jumpSpeed;
    float gravity;
    //std::vector<Bullet*> bullets;

    BulletPool bulletPool;





public:
    vector<std::shared_ptr<Bullet>>bullets;



    // 총알 리스트
    //std::vector<std::unique_ptr<Bullet>> bullets;


    int defeatedEnemies = 0; // 적 제거 카운트


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


    BulletPool& GetBulletPool();

    int GetDefeatedEnemies() const; // 적 제거 카운트 반환
    void IncrementDefeatedEnemies(); // 적 제거 카운트 증가


    //void UpdateBullets(float deltaTime); // 총알 업데이트
    // 총알 리스트 반환
    //std::vector<std::unique_ptr<Bullet>>& GetBullets();


    //void Stop();
    //void ResetSpeed();
    //const std::vector<Bullet*>& GetBullets() const;


    //glm::vec3 GetPosition();
    //void SetDirection(const glm::vec3& dir);
    //glm::vec3 GetDirection() const;

    // 캐릭터 y축 회전값
    float GetYaw() const;
    void SetYaw(float _yaw);

    // 렌더링 함수 (Actor의 Render를 오버라이드)
    void Render(GLuint shaderProgram) override;


    void SetForwardVector(glm::vec3 forwardVector);


    void CheckBulletPoolStatus() {
        auto& bulletPool = GetBulletPool();

        // 풀에 있는 전체 총알 수
       // std::cout << "Total Bullets in Pool: " << bulletPool.GetAllBullets().size() << std::endl;

        // 사용 가능한 총알 수
       // std::cout << "Available Bullets: " << bulletPool.GetAvailableBulletCount() << std::endl;
    }
};