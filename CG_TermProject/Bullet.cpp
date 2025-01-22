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
    moveSpeed(30.f)
{
	//std::cout << "Bullet object created!" << std::endl;
}

void Bullet::Update(float deltaTime) {
    if (isActive) {
        position += direction * moveSpeed * deltaTime;
    }
}

bool Bullet::IsActive() const {
    return isActive;
}

void Bullet::DeActivate() {
    isActive = false;
}

void Bullet::Activate(const glm::vec3& position, const glm::vec3& newDirection)
{
    isActive = true;
    SetPosition(position);
    direction = glm::normalize(newDirection);
}




// ==================================


BulletPool::BulletPool(size_t size) : poolSize(size)
{
	for (size_t i{}; i < poolSize; ++i) {
		pool.emplace_back(make_shared<Bullet>());
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

void BulletPool::UpdateAllBullets(float deltaTime, glm::vec3 startPos)
{
    for (auto& bullet : pool) {
        if (bullet->IsActive()) { // Ȱ��ȭ�� �Ѿ˸� ������Ʈ
            bullet->Update(deltaTime);

            // startPos�κ��� ���� ��ġ������ �Ÿ� ���
            float distance = glm::distance(startPos, bullet->GetPosition());

            // �Ÿ��� 30�� �ʰ��ϸ� ��Ȱ��ȭ
            if (distance > 30.0f) {
                bullet->DeActivate();
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