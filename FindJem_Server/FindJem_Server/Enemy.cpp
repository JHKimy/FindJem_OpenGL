#include "AStarPath.h"
#include "Enemy.h"
#include <iostream>

// ===== ������ =====
Enemy::Enemy(float x, float y, float z, int id) : enemyID{id}
{
    isActive = true;
    
    position.x = x;
    position.y = y;
    position.z = z;

    health = 3;
    moveSpeed = 0.5f;

    currentState = EnemyState::Patrol;

    detectionRadius = 20.0f;
    chaseRadius = 30.0f;

 }

// ===== ���� ���� =====
void Enemy::Patrol(const std::vector<std::vector<int>>& mazeMap) {


    // �̷� ��ǥ ��ȯ
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
            currentDir = -currentDir; // �ݴ� ����
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

// ===== ���� ���� =====
void Enemy::Chase(const glm::vec3& playerPosition, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize, float deltaTime) {

    if (currentPathIndex >= path.size()) {
        detectPath = false;  // ��ΰ� �����ٸ� ��� ������Ʈ �ʿ�
        return;
    }

    float nextX = path[currentPathIndex].x * blockSize.x;
    float nextZ = path[currentPathIndex].y * blockSize.z;

    glm::vec3 nextPos = { nextX, 0, nextZ };
    glm::vec3 moveDir = glm::normalize(nextPos - position);

    position += moveDir * moveSpeed * deltaTime;


    if (glm::distance(position, nextPos) < 0.1f) {
        currentPathIndex++;
    }
}






//// ===== ��ȿ Ÿ�� Ȯ�� =====
//bool Enemy::isValid(int x, int z, const std::vector<std::vector<int>>& mazeMap) {
//    return x >= 0 && z >= 0 &&
//        x < mazeMap[0].size() && z < mazeMap.size() &&
//        mazeMap[z][x] == 0;
//}
