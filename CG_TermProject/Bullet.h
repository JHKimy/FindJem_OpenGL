#pragma once
#include "Actor.h"
#include <vector>
#include <memory> // ����Ʈ ������ ���


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



// ������Ʈ Ǯ
class BulletPool
{
private:
    std::vector<std::shared_ptr<Bullet>> pool;
    size_t poolSize;

public:
    BulletPool(size_t poolSize);

    // ��Ȱ��ȭ �� �Ѿ� ��ȯ
    std::shared_ptr<Bullet> GetBullet();
    void UpdateAllBullets(float deltaTime);
    const std::vector<std::shared_ptr<Bullet>>& GetAllBullets() const;




    // ��� �Ѿ� ��ȯ
    //const std::vector<std::shared_ptr<Bullet>>& GetAllBullets() const;

    // ��� ������ �Ѿ�(��Ȱ��ȭ�� �Ѿ�) ���� ��ȯ
    size_t GetAvailableBulletCount() const;
};