#pragma once
#include "Actor.h"
#include <memory>
#include <queue>
#include <vector>

enum class EnemyState {
    Patrol, // ���� ����
    Chase   // ���� ����
};

class Enemy : public Actor {
public:
    // ===== ������ =====
    Enemy(const glm::vec3& position);

    // ===== �ֿ� �Լ� =====
    void Update(float deltaTime, const glm::vec3& playerPosition, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize);
    bool IsActive() const;
    void TakeDamage(int amount, const glm::vec3& bulletDirection);
    bool CheckCollisionWithActors(const std::vector<std::unique_ptr<Actor>>& actors, glm::vec3 blockSize);
    glm::vec3 GetDirection() const { return direction; } // �̵� ���� ��ȯ

private:
    // ===== ���� ��ƿ��Ƽ �Լ� =====
    void Patrol(const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize, float deltaTime);
    void Chase(const glm::vec3& playerPosition, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize, float deltaTime);
    bool isValid(int x, int z, const std::vector<std::vector<int>>& mazeMap);

    // ===== ���� ���� ���� =====
    int health;                  // �� ü��
    bool isActive;               // Ȱ�� ����
    float moveSpeed;             // �̵� �ӵ�
    EnemyState currentState;     // ���� ����
    glm::vec3 direction;         // �̵� ����

    // ===== ���� ���� ���� =====
    glm::ivec2 currentDir;       // ���� �̵� ����
    glm::ivec2 targetTile;       // ��ǥ Ÿ�� ��ǥ
    glm::vec3 targetPosition;    // ��ǥ ���� ��ǥ

    // ===== Ž�� �� ���� ���� ���� =====
    float detectionRadius;       // Ž�� �Ÿ�
    float chaseRadius;           // ���� ���� �Ÿ�

    // ===== ���� �� �߷� ���� ���� =====
    bool isJumping;              // ���� ����
    float jumpSpeed;             // ���� �ӵ�
    const float gravity = 20.0f; // �߷� ���ӵ�
};
