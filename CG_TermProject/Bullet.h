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
    bool IsActive() const { return isActive; }
    void DeActivate();
    void Activate(const glm::vec3& position, const glm::vec3& newDirection);


    void SetActive(bool a) {isActive = a;}


    glm::vec3 GetDirection() const{return direction;}
};



// ������Ʈ Ǯ
class BulletPool
{
private:
    std::vector<std::unique_ptr<Bullet>> pool;
    int poolSize;

public:
    BulletPool(int poolSize);
    
    // ��Ȱ��ȭ �� �Ѿ� ��ȯ
    Bullet* GetBullet();
    void UpdateAllBullets(float deltaTime, glm::vec3 startPos);
    const std::vector<std::unique_ptr<Bullet>>& GetAllBullets() const;




    // ��� �Ѿ� ��ȯ
    //const std::vector<std::shared_ptr<Bullet>>& GetAllBullets() const;

    // ��� ������ �Ѿ�(��Ȱ��ȭ�� �Ѿ�) ���� ��ȯ
    //int GetAvailableBulletCount() const;

};