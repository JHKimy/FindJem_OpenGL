#pragma once
#include "Actor.h"
#include"memory"

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

    glm::vec3 patrolStart;  // 순찰 시작 위치
    glm::vec3 patrolEnd;    // 순찰 끝 위치
    glm::vec3 homePosition; // 적의 기본 위치

    EnemyState currentState;// 상태 
    float detectionRadius;  // 탐지 반경
    float chaseRadius;      // 추적 반경


    float jumpSpeed;
    bool isJumping;
    const float gravity = 20.f;






public:
    Enemy(const glm::vec3& position);

    void Move();
    void Update(float deltaTime, const glm::vec3& playerPosition, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize);
    void TakeDamage(int amount, const glm::vec3& bulletDirection);
    bool IsActive() const;
    void SetPatrolPoints(const glm::vec3& start, const glm::vec3& end);
    float GetBoundingRadius();

    bool CheckCollisionWithActors(
        const std::vector<std::unique_ptr<Actor>>& actors,
        glm::vec3 blockSize);
    glm::vec3 GetDirection();


    void Patrol(const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize, int mazeX, int mazeZ, float deltaTime);
    void Chase(const glm::vec3& playerPosition, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize, int mazeX, int mazeZ, float deltaTime);
    bool isValid(int x, int z, const std::vector<std::vector<int>>& mazeMap);



    glm::vec3 FindAlternativeDirection(const glm::vec3& blockPosition, const glm::vec3& blockSize);
    void ChangeDirectionRandomly(int currentMazeX, int currentMazeZ, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize);

};