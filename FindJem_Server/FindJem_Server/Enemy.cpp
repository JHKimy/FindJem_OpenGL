#include "AStarPath.h"
#include "Enemy.h"
#include <iostream>

// ===== ������ =====
Enemy::Enemy(float x, float y, float z)
{
    health = 3;
    moveSpeed = 0.5f;
    isActive = true,
    moveSpeed = 10.f;
    currentState = EnemyState::Patrol;

    detectionRadius = 20.0f;
    chaseRadius = 30.0f;

    position.x = x;
    position.y = y;
    position.z = z;
}

// ===== ���� Ȯ�� =====
bool Enemy::IsActive() {
    return isActive;
}

//// ===== ��ȿ Ÿ�� Ȯ�� =====
//bool Enemy::isValid(int x, int z, const std::vector<std::vector<int>>& mazeMap) {
//    return x >= 0 && z >= 0 &&
//        x < mazeMap[0].size() && z < mazeMap.size() &&
//        mazeMap[z][x] == 0;
//}
