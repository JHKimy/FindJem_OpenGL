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
        pool.back()->Deactivate(); // ��Ȱ��ȭ �� ���·� �ʱ�ȭ
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

void BulletPool::UpdateAllBullets(float deltaTime)
{
    for (auto& bullet : pool) {
        if (bullet->IsActive()) { // Ȱ��ȭ�� �Ѿ˸� ������Ʈ
            bullet->Update(deltaTime);

            // �߰��� �浹 ó���� ��Ȱ��ȭ ������ Ȯ���� �� ����
            if (glm::length(bullet->GetPosition()) > 5.0f) { // ��: ȭ�� ��� �ʰ�
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
        if (!bullet->IsActive()) { // ��Ȱ��ȭ�� �Ѿ˸� ī��Ʈ
            ++count;
        }
    }
    return count;
}