#pragma once
#include "Actor.h"

enum class EnemyState {
    Patrol,   // 패트롤 상태
    Chase     // 추적 상태
};

class Enemy : public Actor {
private:
    int health;
    bool isActive;
    glm::vec3 direction;
    float moveSpeed;

    glm::vec3 patrolStart; // 패트롤 시작 위치
    glm::vec3 patrolEnd;   // 패트롤 끝 위치
    glm::vec3 homePosition; // 적의 기본 위치 (패트롤 복귀 지점)

    EnemyState currentState; // 현재 상태
    float detectionRadius;   // 캐릭터 탐지 반경
    float chaseRadius;       // 추적 반경






public:
    Enemy(const glm::vec3& position);

    void Move();
    void Update(float deltaTime, const glm::vec3& playerPosition, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize);
    void TakeDamage(int amount);
    bool IsActive() const;
    void SetPatrolPoints(const glm::vec3& start, const glm::vec3& end);
};
