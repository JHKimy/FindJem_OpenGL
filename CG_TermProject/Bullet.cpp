#include "Bullet.h"
#include<iostream>

Bullet::Bullet() : Actor
(
	"Cube.obj",
	position,
	glm::vec3(0.1f),
	glm::vec3(0.0f),
	glm::vec3(1.0f)
),  isActive(false),
    direction(glm::vec3(0.f)),
    moveSpeed(5.f)
{
	std::cout << "Bullet object created!" << std::endl;
}

void Bullet::Update(float deltaTime) {
    if (isActive) {
        position += direction * moveSpeed * deltaTime;
    }
}

bool Bullet::IsActive() const {
    return isActive;
}

void Bullet::Deactivate() {
    isActive = false;
}

void Bullet::Activate(const glm::vec3& position, const glm::vec3& newDirection)
{
    isActive = true;
    SetPosition(position);
    direction = glm::normalize(newDirection);
}

bool Bullet::CheckCollision(const glm::vec3& targetPos, float targetRadius)
{
    float distance = glm::length(position - targetPos);
    return distance < targetRadius;
}




// ==================================


BulletPool::BulletPool(size_t size) : poolSize(size)
{
	for (size_t i{}; i < poolSize; ++i) {
		pool.emplace_back(make_shared<Bullet>());
        pool.back()->Deactivate(); // 비활성화 된 상태로 초기화
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

void BulletPool::UpdateAllBullets(float deltaTime)
{
    for (auto& bullet : pool) {
        if (bullet->IsActive()) { // 활성화된 총알만 업데이트
            bullet->Update(deltaTime);

            // 추가로 충돌 처리나 비활성화 조건을 확인할 수 있음
            if (glm::length(bullet->GetPosition()) > 5.0f) { // 예: 화면 경계 초과
                bullet->Deactivate();
            }
        }
    }
}

const std::vector<std::shared_ptr<Bullet>>& BulletPool::GetAllBullets() const
{
    return pool;
}

size_t BulletPool::GetAvailableBulletCount() const {
    size_t count = 0;
    for (const auto& bullet : pool) {
        if (!bullet->IsActive()) { // 비활성화된 총알만 카운트
            ++count;
        }
    }
    return count;
}