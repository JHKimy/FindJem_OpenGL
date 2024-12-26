#pragma once
#include "Actor.h"
#include"memory"
#include <queue> // std::queue ����� ���� �ʿ�

enum class EnemyState {
    Patrol,   
    Chase    
};

class Enemy : public Actor 
{
public:
    Enemy(const glm::vec3& position);

    void InitializePatrolPath(const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize);
    void Update(float deltaTime, const glm::vec3& playerPosition, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize);
    bool IsActive() const;
    void TakeDamage(int amount, const glm::vec3& bulletDirection);

private:
    void Patrol(const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize, float deltaTime);
    void Chase(const glm::vec3& playerPosition, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize, float deltaTime);
    std::vector<glm::vec3> CalculatePatrolPath(const glm::vec3& startPosition, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize);
    bool isValid(int x, int z, const std::vector<std::vector<int>>& mazeMap);

    // Enemy attributes
    int health;
    bool isActive;
    float moveSpeed;
    EnemyState currentState;
    float detectionRadius;
    float chaseRadius;
    glm::vec3 homePosition;
    float boundingRadius;
    float jumpSpeed;
    float gravity = 20.f;
    bool isJumping;
    glm::vec3 direction;
    bool isMoving = false;     // �̵� �� ����
    glm::vec3 targetPosition; // ��ǥ ��ġ

    //int currentPathIndex{}; // ���� �̵� ������ ��Ÿ��


    // Patrol attributes
    std::vector<glm::vec3> patrolPath;
    size_t currentPathIndex;












    //std::vector<glm::vec3> CalculatePathBFS(
    //    const glm::ivec2& start,
    //    const glm::ivec2& goal,
    //    const std::vector<std::vector<int>>& mazeMap,
    //    const glm::vec3& blockSize)
    //{
    //    std::vector<std::vector<bool>> visited(
    //        mazeMap.size(), std::vector<bool>(mazeMap[0].size(), false));
    //    std::queue<std::pair<glm::ivec2, std::vector<glm::ivec2>>> queue;

    //    const std::vector<glm::ivec2> directions = {
    //        {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

    //    queue.push({ start, {start} });
    //    visited[start.y][start.x] = true;

    //    while (!queue.empty())
    //    {
    //        auto [current, path] = queue.front();
    //        queue.pop();

    //        // ��ǥ�� �����ϸ� ��� ��ȯ
    //        if (current == goal)
    //        {
    //            std::vector<glm::vec3> worldPath;
    //            for (const auto& tile : path)
    //            {
    //                worldPath.push_back(glm::vec3(
    //                    tile.x * blockSize.x,
    //                    0.0f,
    //                    tile.y * blockSize.z));
    //            }
    //            return worldPath;
    //        }

    //        // 4���� Ž��
    //        for (const auto& dir : directions)
    //        {
    //            glm::ivec2 next = current + dir;

    //            if (next.x >= 0 && next.x < mazeMap[0].size() &&
    //                next.y >= 0 && next.y < mazeMap.size() &&
    //                mazeMap[next.y][next.x] == 0 && !visited[next.y][next.x])
    //            {
    //                visited[next.y][next.x] = true;
    //                auto newPath = path;
    //                newPath.push_back(next);
    //                queue.push({ next, newPath });
    //            }
    //        }
    //    }

    //    return {}; // ��θ� ã�� ������ ��� �� ��� ��ȯ
    //}

    //glm::vec3 SafeNormalize(const glm::vec3& v)
    //{
    //    float length = glm::length(v);
    //    if (length == 0.0f)
    //        return glm::vec3(0.0f); // �⺻�� ��ȯ (����)
    //    return v / length;
    //}

};