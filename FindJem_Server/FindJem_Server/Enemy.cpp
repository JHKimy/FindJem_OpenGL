#include "AStarPath.h"
#include "Enemy.h"
#include <iostream>
#include "pch.h"

// ===== 생성자 =====
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

// ===== 순찰 동작 =====
void Enemy::Patrol(const std::vector<std::vector<int>>& mazeMap) {


    // 적 위치 미로 좌표 변환
    int mazeX = static_cast<int>(position.x / g_blockSize.x);
    int mazeZ = static_cast<int>(position.z / g_blockSize.z);

    // 타겟 위치와 적 위치 간격 짧아지면 탐색
    if (std::sqrt(
        std::pow(targetPosition.x - position.x, 2) +
        std::pow(targetPosition.z - position.z, 2)) < 0.1f)
    {
        // 4방향
        const std::vector<vec2> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
        bool foundNext = false;

        // 4방향 탐색
        for (const auto& dir : directions) {
            // 다음 미로 좌표
            int nextX = mazeX + dir.x;
            int nextZ = mazeZ + dir.z;

            cout << dir.x << "," << dir.z << endl;

            // 다음 미로 좌표 가능한지 && 현재 방향과 반대편방향은 아닌지
            // 막혔을때 그 방향 좌우 살피기 위함
            if (isValid(nextX, nextZ, mazeMap) && (dir.x != -currentDir.x || dir.z != -currentDir.z))
            {
                currentDir = dir;
                targetTile = { (float)nextX, (float)nextZ };
                foundNext = true;
                break;
            }
        }
        // 가는 방향, 좌, 우 다 안되면 반대 방향
        if (!foundNext) 
        {
            currentDir = { -currentDir.x, -currentDir.z }; // 반대 방향
            targetTile = { mazeX + currentDir.x, mazeZ + currentDir.z };
        }

        // 월드 목표위치로 변환
        targetPosition = { targetTile.x * g_blockSize.x, position.y, targetTile.z * g_blockSize.z };
    }

    // 목표 위치를 향한 방향 계산
    vec3 directionToTarget = {
        targetPosition.x - position.x,
        targetPosition.y - position.y,
        targetPosition.z - position.z
    };

    // 방향 벡터 정규화
    float magnitude = std::sqrt(
        std::pow(directionToTarget.x, 2) +
        std::pow(directionToTarget.y, 2) +
        std::pow(directionToTarget.z, 2));

    if (magnitude > 0.0f) {
        directionToTarget.x /= magnitude;
        directionToTarget.y /= magnitude;
        directionToTarget.z /= magnitude;
    }

    // 이동
    position.x += directionToTarget.x * moveSpeed ;
    position.y += directionToTarget.y * moveSpeed ;
    position.z += directionToTarget.z * moveSpeed ;


    // 목표 위치에 도달했는지 확인
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
    g_enemies[enemyIndex]->detectPath = true;  // 경로가 업데이트되었음을 표시


    // 경로가 계산된 결과 출력
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

// ===== 추적 동작 =====
void Enemy::Chase() 
{
    
    if (path.empty()) {
        //cout << "[DEBUG] Chase: Path is empty!" << endl;
        return;
    }

    //cout << "[DEBUG] Starting Chase. Current Path Index: " << currentPathIndex << endl;
    while (currentPathIndex < path.size()) {
        // 다음 타일 좌표 계산
        float nextX = path[currentPathIndex].x * g_blockSize.x;
        float nextZ = path[currentPathIndex].z * g_blockSize.z;
        vec3 nextPos = { nextX, 0, nextZ };

        //cout << "[DEBUG] Current Position: (" << position.x << ", " << position.z << ")" << endl;
        //cout << "[DEBUG] Target Position: (" << nextPos.x << ", " << nextPos.z << ")" << endl;

        // 방향 벡터 계산
        vec3 moveDir = normalize(position, nextPos);

        // 이동
        position.x += moveDir.x * moveSpeed;
        position.y += moveDir.y * moveSpeed;
        position.z += moveDir.z * moveSpeed;

        // 목표 위치에 도달했는지 확인
        float distanceToNext = std::sqrt(
            std::pow(nextPos.x - position.x, 2) +
            std::pow(nextPos.y - position.y, 2) +
            std::pow(nextPos.z - position.z, 2));

        //cout << "[DEBUG] Distance to Next: " << distanceToNext << endl;

        if (distanceToNext < 0.1f) {
            currentPathIndex++; // 다음 타일로 이동
            //cout << "[DEBUG] Moving to Next Index: " << currentPathIndex << endl;
        }
        else {
            break; // 아직 다음 타일에 도달하지 못했으므로 루프 종료
        }
    }

    // 경로 끝에 도달했는지 확인
    if (currentPathIndex >= path.size()) {
        detectPath = false; // 경로가 끝났음을 표시
       // cout << "[DEBUG] Chase: Reached end of path." << endl;
    }
}






//// ===== 유효 타일 확인 =====
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
