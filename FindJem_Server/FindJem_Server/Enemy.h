#pragma once
#include <memory>
#include <queue>
#include <vector>
#include <utility>
#include <cmath>
#include "Types.h"

enum class EnemyState {
    Patrol, // ���� ����
    Chase   // ���� ����
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
    int     enemyID;       // �÷��̾� ID
    bool    isActive;      // Ȱ�� ����
    int     health;         // ü��
    float   mass;           // ����
    float   moveSpeed;      // ���ǵ�
    bool    isJumping;      // ���� ����



    vec3 position;           // ���� ��ġ
    vec2 currentDir;        // ���� �̵� ����
    vec3 targetPosition;    // Ÿ�� ��ġ
    vec2 targetTile;        // ��ǥ Ÿ�� ��ǥ



    EnemyState currentState;    // ���� ����

    float detectionRadius;      // Ž�� �Ÿ�
    float chaseRadius;          // ���� ���� �Ÿ�

    float   boundingRadius;     // �浹 ��ü ������



    // AStar �˰��� Ž�� ����
    bool detectPath = false;
    
    // AStar �˰��� ���
    std::vector<vec2> path;       
    
    // AStar Path �ε���
    size_t currentPathIndex = 0;


// ===== ������ =====
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
        // ���� ���� ���
        vec3 direction = { to.x - from.x, to.y - from.y, to.z - from.z };

        // ���� ���� ���
        float magnitude = std::sqrt(direction.x * direction.x +
            direction.y * direction.y +
            direction.z * direction.z);

        // ���̰� 0�� ��� ó��
        if (magnitude == 0.0f) {
            return { 0.0f, 0.0f, 0.0f }; // ���̰� 0�� ��� ����ȭ �Ұ���
        }

        // ����ȭ
        return { direction.x / magnitude,
                direction.y / magnitude,
                direction.z / magnitude };
    }



    //// ===== �ֿ� �Լ� =====
    //void Update(float deltaTime, const glm::vec3& playerPosition, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize);
    //bool IsActive() const;
    //void TakeDamage(int amount, const glm::vec3& bulletDirection);
    //glm::vec3 GetDirection() const { return direction; } // �̵� ���� ��ȯ

public:
    // ===== ���� ��ƿ��Ƽ �Լ� =====
    void Patrol(const std::vector<std::vector<int>>& mazeMap);

    void MakeAStarPath(int enemyIndex, int playerIndex);
    void Chase();
    bool isValid(int x, int z, const std::vector<std::vector<int>>& mazeMap);

    float DistanceToPlayer(int enemyIndex, int playerIndex);






};


