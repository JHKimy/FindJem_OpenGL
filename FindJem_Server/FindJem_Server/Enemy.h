#pragma once
#include <memory>
#include <queue>
#include <vector>
#include <utility>
#include <cmath>
#include "Types.h"

enum class EnemyState {
    Patrol, // 순찰 상태
    Chase   // 추적 상태
};


//struct vec3 {
//    float x, y, z;
//};
//
//struct vec2 {
//    float x, z;
//};

class Enemy
{
public:
    int     enemyID;       // 플레이어 ID
    bool    isActive;      // 활성 상태
    int     health;         // 체력
    float   mass;           // 무게
    float   moveSpeed;      // 스피드
    bool    isJumping;      // 점프 변수



    vec3 position;           // 현재 위치
    vec2 currentDir;        // 현재 이동 방향
    vec3 targetPosition;    // 타겟 위치
    vec2 targetTile;        // 목표 타일 좌표



    EnemyState currentState;    // 현재 상태

    float detectionRadius;      // 탐지 거리
    float chaseRadius;          // 추적 유지 거리

    float   boundingRadius;     // 충돌 구체 반지름



    // AStar 알고리즘 탐색 여부
    bool detectPath = false;
    
    // AStar 알고리즘 경로
    std::vector<vec2> path;       
    
    // AStar Path 인덱스
    size_t currentPathIndex = 0;


// ===== 생성자 =====
    Enemy(float x, float y, float z, int id);

    bool IsActive() const{ return isActive; };


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

    vec3 normalize(const vec3& from, const vec3& to) {
        // 방향 벡터 계산
        vec3 direction = { to.x - from.x, to.y - from.y, to.z - from.z };

        // 벡터 길이 계산
        float magnitude = std::sqrt(direction.x * direction.x +
            direction.y * direction.y +
            direction.z * direction.z);

        // 길이가 0인 경우 처리
        if (magnitude == 0.0f) {
            return { 0.0f, 0.0f, 0.0f }; // 길이가 0인 경우 정규화 불가능
        }

        // 정규화
        return { direction.x / magnitude,
                direction.y / magnitude,
                direction.z / magnitude };
    }



    //// ===== 주요 함수 =====
    //void Update(float deltaTime, const glm::vec3& playerPosition, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize);
    //bool IsActive() const;
    //void TakeDamage(int amount, const glm::vec3& bulletDirection);
    //glm::vec3 GetDirection() const { return direction; } // 이동 방향 반환

public:
    // ===== 내부 유틸리티 함수 =====
    void Patrol(const std::vector<std::vector<int>>& mazeMap);

    void MakeAStarPath(int enemyIndex, int playerIndex);
    void Chase();
    bool isValid(int x, int z, const std::vector<std::vector<int>>& mazeMap);

    float DistanceToPlayer(int enemyIndex, int playerIndex);






};


