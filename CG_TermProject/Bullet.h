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
    bool IsActive() const { return isActive; }
    void DeActivate();
    void Activate(const glm::vec3& position, const glm::vec3& newDirection);


    void SetActive(bool a) {isActive = a;}


    glm::vec3 GetDirection() const{return direction;}
};



// 오브젝트 풀
class BulletPool
{
private:
    std::vector<std::unique_ptr<Bullet>> pool;
    int poolSize;

public:
    BulletPool(int poolSize);
    
    // 비활성화 된 총알 반환
    Bullet* GetBullet();
    void UpdateAllBullets(float deltaTime, glm::vec3 startPos);
    const std::vector<std::unique_ptr<Bullet>>& GetAllBullets() const;




    // 모든 총알 반환
    //const std::vector<std::shared_ptr<Bullet>>& GetAllBullets() const;

    // 사용 가능한 총알(비활성화된 총알) 개수 반환
    //int GetAvailableBulletCount() const;

};