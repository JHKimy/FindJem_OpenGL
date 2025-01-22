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
        pool.back()->DeActivate(); // ��Ȱ��ȭ �� ���·� �ʱ�ȭ
    }
}

std::shared_ptr<Bullet> BulletPool::GetBullet()
{
    for (auto& bullet : pool) {
        if (!bullet->IsActive()) {
            return bullet; // ��Ȱ��ȭ�� �Ѿ� ��ȯ
        }
    }
}

void BulletPool::UpdateAllBullets( vec3 startPos)
{
    for (auto& bullet : pool) {
        if (bullet->IsActive()) { // Ȱ��ȭ�� �Ѿ˸� ������Ʈ
            bullet->Update();

            // startPos�κ��� ���� ��ġ������ �Ÿ� ���
            float bullet_distance = V::Distance(startPos, bullet->GetPosition());

            // �Ÿ��� 30�� �ʰ��ϸ� ��Ȱ��ȭ
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
        if (!bullet->IsActive()) { // ��Ȱ��ȭ�� �Ѿ˸� ī��Ʈ
            ++count;
        }
    }
    return count;
}