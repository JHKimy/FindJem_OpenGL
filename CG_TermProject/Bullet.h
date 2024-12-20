#pragma once
#include "Actor.h"
#include <vector>
#include <memory> // 스마트 포인터 사용


class Bullet : public Actor {
private:
    bool isActive;
    glm::vec3 direction;
    float moveSpeed;

public:
    Bullet();

    void Update(float deltaTime);
    bool IsActive() const;
    void Deactivate();
    bool CheckCollision(const glm::vec3& targetPos, float targetRadius);
    void Activate(const glm::vec3& position, const glm::vec3& newDirection);
};



// 오브젝트 풀
class BulletPool
{
private:
    std::vector<std::shared_ptr<Bullet>> pool;
    size_t poolSize;

public:
    BulletPool(size_t poolSize);

    // 비활성화 된 총알 반환
    std::shared_ptr<Bullet> GetBullet();
    void UpdateAllBullets(float deltaTime);
    const std::vector<std::shared_ptr<Bullet>>& GetAllBullets() const;




    // 모든 총알 반환
    //const std::vector<std::shared_ptr<Bullet>>& GetAllBullets() const;

    // 사용 가능한 총알(비활성화된 총알) 개수 반환
    size_t GetAvailableBulletCount() const;
};