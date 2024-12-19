#pragma once
#include "Actor.h"

enum class EnemyState {
    Patrol,   
    Chase    
};

class Enemy : public Actor {
private:
    int health;
    bool isActive;
    glm::vec3 direction;
    float moveSpeed;

    glm::vec3 patrolStart;  // ���� ���� ��ġ
    glm::vec3 patrolEnd;    // ���� �� ��ġ
    glm::vec3 homePosition; // ���� �⺻ ��ġ

    EnemyState currentState;// ���� 
    float detectionRadius;  // Ž�� �ݰ�
    float chaseRadius;      // ���� �ݰ�






public:
    Enemy(const glm::vec3& position);

    void Move();
    void Update(float deltaTime, const glm::vec3& playerPosition, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize);
    void TakeDamage(int amount);
    bool IsActive() const;
    void SetPatrolPoints(const glm::vec3& start, const glm::vec3& end);
    float GetBoundingRadius();
};