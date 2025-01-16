#pragma once
#include <memory>
#include <queue>
#include <vector>
#include <utility>

enum class EnemyState {
    Patrol, // 순찰 상태
    Chase   // 추적 상태
};

class Enemy
{
public:
    int     enemyID;       // 플레이어 ID
    bool    isActive;      // 활성 상태
    
    // 위치
    struct Position
    {
        float x, y, z;
    }position;

    int     health;
    float   mass;
    float   moveSpeed;
    bool    isJumping;

    EnemyState currentState;    // 현재 상태

    float detectionRadius;      // 탐지 거리
    float chaseRadius;          // 추적 유지 거리

    float   boundingRadius;     // 충돌 구체 반지름


    // ===== 생성자 =====
    Enemy(float x, float y, float z, int id);

    bool IsActive() { return IsActive; };


    float GetPostionX(){return position.x;}

    float GetPostionY(){
        return position.y;
    }
    float GetPostionZ() {
        return position.z;
    }

    int GetEnemyID() {
        return enemyID;
    }




    //// ===== 주요 함수 =====
    //void Update(float deltaTime, const glm::vec3& playerPosition, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize);
    //bool IsActive() const;
    //void TakeDamage(int amount, const glm::vec3& bulletDirection);
    //glm::vec3 GetDirection() const { return direction; } // 이동 방향 반환

private:
    // ===== 내부 유틸리티 함수 =====
    void Patrol(const std::vector<std::vector<int>>& mazeMap, const Vec3& blockSize, float deltaTime);
    void Chase(const std::pair<int,int> playerPosition, const std::vector<std::vector<int>>& mazeMap, const Vec3& blockSize, float deltaTime);
    //bool isValid(int x, int z, const std::vector<std::vector<int>>& mazeMap);
};


