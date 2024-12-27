#include "Enemy.h"
#include <iostream>

// ===== 생성자 =====
Enemy::Enemy(const glm::vec3& position)
    : Actor(
        "Cube.obj", 
        position, 
        glm::vec3(2.0f), 
        glm::vec3(0.0f), 
        glm::vec3(1.0f, 0.0f, 0.0f)),
    health(3),
    isActive(true),
    moveSpeed(5.f),
    currentState(EnemyState::Patrol),
    detectionRadius(10.0f),
    chaseRadius(20.0f),
    isJumping(false),
    jumpSpeed(0.0f),
    currentDir({ 1, 0 }),
    targetTile({ static_cast<int>(position.x / 5.0f), static_cast<int>(position.z / 5.0f) }) {
    targetPosition = glm::vec3(targetTile.x * 5.0f, 0.0f, targetTile.y * 5.0f);
}

// ===== 상태 업데이트 =====
void Enemy::Update(float deltaTime, const glm::vec3& playerPosition, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize) {
    if (!isActive) return;

    float distanceToPlayer = glm::length(playerPosition - position);

    switch (currentState) {
    case EnemyState::Patrol:
        if (distanceToPlayer < detectionRadius) {
            currentState = EnemyState::Chase;
        }
        else {
            Patrol(mazeMap, blockSize, deltaTime);
        }
        break;

    case EnemyState::Chase:
        if (distanceToPlayer > chaseRadius) {
            currentState = EnemyState::Patrol;
        }
        else {
            Chase(playerPosition, mazeMap, blockSize, deltaTime);
        }
        break;
    }
}

// ===== 순찰 동작 =====
void Enemy::Patrol(const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize, float deltaTime) {
    int mazeX = static_cast<int>(position.x / blockSize.x);
    int mazeZ = static_cast<int>(position.z / blockSize.z);

    if (glm::length(targetPosition - position) < 0.1f) {
        const std::vector<glm::ivec2> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
        bool foundNext = false;

        for (const auto& dir : directions) {
            int nextX = mazeX + dir.x;
            int nextZ = mazeZ + dir.y;

            if (isValid(nextX, nextZ, mazeMap) && dir != -currentDir) {
                currentDir = dir;
                targetTile = { nextX, nextZ };
                foundNext = true;
                break;
            }
        }

        if (!foundNext) {
            currentDir = -currentDir; // 반대 방향
            targetTile = { mazeX + currentDir.x, mazeZ + currentDir.y };
        }

        targetPosition = glm::vec3(targetTile.x * blockSize.x, position.y, targetTile.y * blockSize.z);
    }

    glm::vec3 directionToTarget = glm::normalize(targetPosition - position);
    position += directionToTarget * moveSpeed * deltaTime;

    if (glm::length(targetPosition - position) < 0.1f) {
        position = targetPosition;
    }
}

// ===== 추적 동작 =====
void Enemy::Chase(const glm::vec3& playerPosition, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize, float deltaTime) {
    direction = glm::normalize(playerPosition - position);
    position += direction * moveSpeed * deltaTime;
}

// ===== 충돌 감지 =====
bool Enemy::CheckCollisionWithActors(const std::vector<std::unique_ptr<Actor>>& actors, glm::vec3 blockSize) {
    for (const auto& actor : actors) {
        glm::vec3 actorPos = actor->GetPosition();
        if (glm::distance(this->GetPosition(), actorPos) < blockSize.x) {
            direction = -direction;
            return true;
        }
    }
    return false;
}

// ===== 상태 확인 =====
bool Enemy::IsActive() const {
    return isActive;
}

// ===== 데미지 처리 =====
void Enemy::TakeDamage(int amount, const glm::vec3& bulletDirection) {
    health -= amount;
    if (health <= 0) {
        isActive = false;
    }

    position += bulletDirection * 2.f;
    if (!isJumping) {
        isJumping = true;
        jumpSpeed = 3.0f;
    }
}

// ===== 유효 타일 확인 =====
bool Enemy::isValid(int x, int z, const std::vector<std::vector<int>>& mazeMap) {
    return x >= 0 && z >= 0 &&
        x < mazeMap[0].size() && z < mazeMap.size() &&
        mazeMap[z][x] == 0;
}
