#include "Bullet.h"
#include<iostream>

Bullet::Bullet():
    isActive(false),
    direction(vec3{ 0.f,0.f,0.f }),
    moveSpeed(1.f)
{
}

void Bullet::Update( ) 
{
    if (isActive) {
        position.x += direction.x * moveSpeed * 0.01;
        position.y += direction.y * moveSpeed * 0.01;
        position.z += direction.z * moveSpeed * 0.01;
    }
}

bool Bullet::IsActive() {
    return isActive;
}

void Bullet::DeActivate() {
    isActive = false;
}

void Bullet::Activate( vec3& position,  vec3& newDirection)
{
    isActive = true;
    SetPosition(position);
    direction = V::Normalize(newDirection);
}




// ==================================


BulletPool::BulletPool(size_t size) : poolSize(size)
{
    for (size_t i{}; i < poolSize; ++i) {
        pool.emplace_back(std::make_shared<Bullet>());
        pool.back()->DeActivate(); // 비활성화 된 상태로 초기화
    }
}

std::shared_ptr<Bullet> BulletPool::GetBullet()
{
    for (auto& bullet : pool) {
        if (!bullet->IsActive()) {
            return bullet; // 비활성화된 총알 반환
        }
    }
}

void BulletPool::UpdateAllBullets( vec3 startPos)
{
    for (auto& bullet : pool) {
        if (bullet->IsActive()) { // 활성화된 총알만 업데이트
            bullet->Update();

            // startPos로부터 현재 위치까지의 거리 계산
            float bullet_distance = V::Distance(startPos, bullet->GetPosition());

            // 거리가 30을 초과하면 비활성화
            if (bullet_distance > 30.0f) {
                bullet->DeActivate();
            }
        }
    }
}

std::vector<std::shared_ptr<Bullet>>& BulletPool::GetAllBullets()
{
    return pool;
}

size_t BulletPool::GetAvailableBulletCount() {
    size_t count = 0;
    for (const auto& bullet : pool) {
        if (!bullet->IsActive()) { // 비활성화된 총알만 카운트
            ++count;
        }
    }
    return count;
}