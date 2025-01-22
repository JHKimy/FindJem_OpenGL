#pragma once

#include "Actor.h"
#include "Bullet.h"
#include <vector>
#include <memory> // ����Ʈ ������ ���

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



    // �Ѿ� ����Ʈ
    //std::vector<std::unique_ptr<Bullet>> bullets;


    int defeatedEnemies = 0; // �� ���� ī��Ʈ


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

    int GetDefeatedEnemies() const; // �� ���� ī��Ʈ ��ȯ
    void IncrementDefeatedEnemies(); // �� ���� ī��Ʈ ����


    //void UpdateBullets(float deltaTime); // �Ѿ� ������Ʈ
    // �Ѿ� ����Ʈ ��ȯ
    //std::vector<std::unique_ptr<Bullet>>& GetBullets();


    //void Stop();
    //void ResetSpeed();
    //const std::vector<Bullet*>& GetBullets() const;


    //glm::vec3 GetPosition();
    //void SetDirection(const glm::vec3& dir);
    //glm::vec3 GetDirection() const;

    // ĳ���� y�� ȸ����
    float GetYaw() const;
    void SetYaw(float _yaw);

    // ������ �Լ� (Actor�� Render�� �������̵�)
    void Render(GLuint shaderProgram) override;


    void SetForwardVector(glm::vec3 forwardVector);


    void CheckBulletPoolStatus() {
        auto& bulletPool = GetBulletPool();

        // Ǯ�� �ִ� ��ü �Ѿ� ��
       // std::cout << "Total Bullets in Pool: " << bulletPool.GetAllBullets().size() << std::endl;

        // ��� ������ �Ѿ� ��
       // std::cout << "Available Bullets: " << bulletPool.GetAvailableBulletCount() << std::endl;
    }
};