#pragma once
#include <memory>
#include <queue>
#include <vector>
#include <utility>

enum class EnemyState {
    Patrol, // ���� ����
    Chase   // ���� ����
};

class Enemy
{
public:
    int     enemyID;       // �÷��̾� ID
    bool    isActive;      // Ȱ�� ����
    
    // ��ġ
    struct Position
    {
        float x, y, z;
    }position;

    int     health;
    float   mass;
    float   moveSpeed;
    bool    isJumping;

    EnemyState currentState;    // ���� ����

    float detectionRadius;      // Ž�� �Ÿ�
    float chaseRadius;          // ���� ���� �Ÿ�

    float   boundingRadius;     // �浹 ��ü ������


    // ===== ������ =====
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




    //// ===== �ֿ� �Լ� =====
    //void Update(float deltaTime, const glm::vec3& playerPosition, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize);
    //bool IsActive() const;
    //void TakeDamage(int amount, const glm::vec3& bulletDirection);
    //glm::vec3 GetDirection() const { return direction; } // �̵� ���� ��ȯ

private:
    // ===== ���� ��ƿ��Ƽ �Լ� =====
    void Patrol(const std::vector<std::vector<int>>& mazeMap, const Vec3& blockSize, float deltaTime);
    void Chase(const std::pair<int,int> playerPosition, const std::vector<std::vector<int>>& mazeMap, const Vec3& blockSize, float deltaTime);
    //bool isValid(int x, int z, const std::vector<std::vector<int>>& mazeMap);
};


