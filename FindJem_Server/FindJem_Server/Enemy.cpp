#include "AStarPath.h"
#include "Enemy.h"
#include <iostream>
#include "pch.h"

// ===== ������ =====
Enemy::Enemy(float x, float y, float z, int id) : enemyID{ id }, currentDir{1,0}
{
    isActive = true;
    
    position.x = x;
    position.y = y;
    position.z = z;

    health = 3;
    moveSpeed = 0.5f;

    currentState = EnemyState::Patrol;

    detectionRadius = 20.0f;
    chaseRadius = 30.0f;
    targetTile.x = static_cast<int>(position.x / 5.0f);

    targetTile.z = static_cast<int>(position.z / 5.0f);
    
    targetPosition.x = targetTile.x * 5.0f;
    targetPosition.y = 0.0f;
    targetPosition.z = targetTile.z * 5.0f;

    detectPath = false;
 }

// ===== ���� ���� =====
void Enemy::Patrol(const std::vector<std::vector<int>>& mazeMap) {


    // �� ��ġ �̷� ��ǥ ��ȯ
    int mazeX = static_cast<int>(position.x / g_blockSize.x);
    int mazeZ = static_cast<int>(position.z / g_blockSize.z);

    // Ÿ�� ��ġ�� �� ��ġ ���� ª������ Ž��
    if (std::sqrt(
        std::pow(targetPosition.x - position.x, 2) +
        std::pow(targetPosition.z - position.z, 2)) < 0.1f)
    {
        // 4����
        const std::vector<vec2> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
        bool foundNext = false;

        // 4���� Ž��
        for (const auto& dir : directions) {
            // ���� �̷� ��ǥ
            int nextX = mazeX + dir.x;
            int nextZ = mazeZ + dir.z;

            cout << dir.x << "," << dir.z << endl;

            // ���� �̷� ��ǥ �������� && ���� ����� �ݴ�������� �ƴ���
            // �������� �� ���� �¿� ���Ǳ� ����
            if (isValid(nextX, nextZ, mazeMap) && (dir.x != -currentDir.x || dir.z != -currentDir.z))
            {
                currentDir = dir;
                targetTile = { (float)nextX, (float)nextZ };
                foundNext = true;
                break;
            }
        }
        // ���� ����, ��, �� �� �ȵǸ� �ݴ� ����
        if (!foundNext) 
        {
            currentDir = { -currentDir.x, -currentDir.z }; // �ݴ� ����
            targetTile = { mazeX + currentDir.x, mazeZ + currentDir.z };
        }

        // ���� ��ǥ��ġ�� ��ȯ
        targetPosition = { targetTile.x * g_blockSize.x, position.y, targetTile.z * g_blockSize.z };
    }

    // ��ǥ ��ġ�� ���� ���� ���
    vec3 directionToTarget = {
        targetPosition.x - position.x,
        targetPosition.y - position.y,
        targetPosition.z - position.z
    };

    // ���� ���� ����ȭ
    float magnitude = std::sqrt(
        std::pow(directionToTarget.x, 2) +
        std::pow(directionToTarget.y, 2) +
        std::pow(directionToTarget.z, 2));

    if (magnitude > 0.0f) {
        directionToTarget.x /= magnitude;
        directionToTarget.y /= magnitude;
        directionToTarget.z /= magnitude;
    }

    // �̵�
    position.x += directionToTarget.x * moveSpeed ;
    position.y += directionToTarget.y * moveSpeed ;
    position.z += directionToTarget.z * moveSpeed ;


    // ��ǥ ��ġ�� �����ߴ��� Ȯ��
    float distanceToTarget = std::sqrt(
        std::pow(targetPosition.x - position.x, 2) +
        std::pow(targetPosition.y - position.y, 2) +
        std::pow(targetPosition.z - position.z, 2));

    if (distanceToTarget < 0.1f) {
        position = targetPosition;
    }

}

void Enemy::MakeAStarPath(int enemyIndex, int playerIndex)
{
    float tempX = static_cast<int>(g_enemies[enemyIndex]->position.x / g_blockSize.x);
    float tempZ = static_cast<int>(g_enemies[enemyIndex]->position.z / g_blockSize.z);
    vec2 startTile = { tempX, tempZ };



    float tempX2 = static_cast<int>(g_characters[playerIndex].GetPostionX() / g_blockSize.x);
    float tempZ2 = static_cast<int>(g_characters[playerIndex].GetPostionZ() / g_blockSize.z);
    vec2 goalTile = { tempX2,tempZ2 };

    cout << "[DEBUG] Character Position: (" << g_characters[playerIndex].GetPostionX()
        << ", " << g_characters[playerIndex].GetPostionZ() << ")" << endl;
    cout << "[DEBUG] Goal Tile: (" << goalTile.x << ", " << goalTile.z << ")" << endl;

    g_enemies[enemyIndex]->path = Astar::FindPath(startTile, goalTile, g_mazeMap);

    g_enemies[enemyIndex]->currentPathIndex = 0;
    g_enemies[enemyIndex]->detectPath = true;  // ��ΰ� ������Ʈ�Ǿ����� ǥ��


    // ��ΰ� ���� ��� ���
    if (g_enemies[enemyIndex]->path.empty()) {
        cout << "[DEBUG] Pathfinding failed! No path found." << endl;
    }
    else {
        cout << "[DEBUG] Path found:" << endl;
        for (auto& p : g_enemies[enemyIndex]->path) {
            cout << "  Tile: (" << p.x << ", " << p.z << ")" << endl;
        }
    }
}

// ===== ���� ���� =====
void Enemy::Chase() 
{
    
    if (path.empty()) {
        //cout << "[DEBUG] Chase: Path is empty!" << endl;
        return;
    }

    //cout << "[DEBUG] Starting Chase. Current Path Index: " << currentPathIndex << endl;
    while (currentPathIndex < path.size()) {
        // ���� Ÿ�� ��ǥ ���
        float nextX = path[currentPathIndex].x * g_blockSize.x;
        float nextZ = path[currentPathIndex].z * g_blockSize.z;
        vec3 nextPos = { nextX, 0, nextZ };

        //cout << "[DEBUG] Current Position: (" << position.x << ", " << position.z << ")" << endl;
        //cout << "[DEBUG] Target Position: (" << nextPos.x << ", " << nextPos.z << ")" << endl;

        // ���� ���� ���
        vec3 moveDir = normalize(position, nextPos);

        // �̵�
        position.x += moveDir.x * moveSpeed;
        position.y += moveDir.y * moveSpeed;
        position.z += moveDir.z * moveSpeed;

        // ��ǥ ��ġ�� �����ߴ��� Ȯ��
        float distanceToNext = std::sqrt(
            std::pow(nextPos.x - position.x, 2) +
            std::pow(nextPos.y - position.y, 2) +
            std::pow(nextPos.z - position.z, 2));

        //cout << "[DEBUG] Distance to Next: " << distanceToNext << endl;

        if (distanceToNext < 0.1f) {
            currentPathIndex++; // ���� Ÿ�Ϸ� �̵�
            //cout << "[DEBUG] Moving to Next Index: " << currentPathIndex << endl;
        }
        else {
            break; // ���� ���� Ÿ�Ͽ� �������� �������Ƿ� ���� ����
        }
    }

    // ��� ���� �����ߴ��� Ȯ��
    if (currentPathIndex >= path.size()) {
        detectPath = false; // ��ΰ� �������� ǥ��
       // cout << "[DEBUG] Chase: Reached end of path." << endl;
    }
}






//// ===== ��ȿ Ÿ�� Ȯ�� =====
bool Enemy::isValid(int x, int z, const std::vector<std::vector<int>>& mazeMap) {
    return x >= 0 && z >= 0 &&
        x < mazeMap[0].size() && z < mazeMap.size() &&
        mazeMap[z][x] == 0;
}

float Enemy::DistanceToPlayer(int enemyIndex, int playerIndex)
{
    return std::sqrt(
        std::pow(g_characters[playerIndex].position.x - g_enemies[enemyIndex]->position.x, 2) +
        std::pow(g_characters[playerIndex].position.z - g_enemies[enemyIndex]->position.z, 2));

}
