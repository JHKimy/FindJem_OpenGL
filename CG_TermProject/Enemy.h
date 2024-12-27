#pragma once
#include "Actor.h"
#include <memory>
#include <queue>
#include <vector>

enum class EnemyState {
    Patrol, // 순찰 상태
    Chase   // 추적 상태
};

class Enemy : public Actor {
public:
    // ===== 생성자 =====
    Enemy(const glm::vec3& position);

    // ===== 주요 함수 =====
    void Update(float deltaTime, const glm::vec3& playerPosition, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize);
    bool IsActive() const;
    void TakeDamage(int amount, const glm::vec3& bulletDirection);
    bool CheckCollisionWithActors(const std::vector<std::unique_ptr<Actor>>& actors, glm::vec3 blockSize);
    glm::vec3 GetDirection() const { return direction; } // 이동 방향 반환

private:
    // ===== 내부 유틸리티 함수 =====
    void Patrol(const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize, float deltaTime);
    void Chase(const glm::vec3& playerPosition, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize, float deltaTime);
    bool isValid(int x, int z, const std::vector<std::vector<int>>& mazeMap);

    // ===== 상태 관리 변수 =====
    int health;                  // 적 체력
    bool isActive;               // 활성 상태
    float moveSpeed;             // 이동 속도
    EnemyState currentState;     // 현재 상태
    glm::vec3 direction;         // 이동 방향

    // ===== 순찰 관련 변수 =====
    glm::ivec2 currentDir;       // 현재 이동 방향
    glm::ivec2 targetTile;       // 목표 타일 좌표
    glm::vec3 targetPosition;    // 목표 월드 좌표

    // ===== 탐지 및 추적 관련 변수 =====
    float detectionRadius;       // 탐지 거리
    float chaseRadius;           // 추적 유지 거리

    // ===== 점프 및 중력 관련 변수 =====
    bool isJumping;              // 점프 여부
    float jumpSpeed;             // 점프 속도
    const float gravity = 20.0f; // 중력 가속도
};
