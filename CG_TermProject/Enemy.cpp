#include "Enemy.h"
#include <random>

Enemy::Enemy(const glm::vec3& position)
    : Actor("Cube.obj",
        position, 
        glm::vec3(2.0f),
        glm::vec3(0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f)),
    health(3),
    isActive(true),
    moveSpeed(5.f),
    currentState(EnemyState::Patrol),
    detectionRadius(10.0f),
    chaseRadius(20.0f),
    homePosition(position),
    isJumping(false),
    jumpSpeed(0.0f)
{
    // 초기 위치와 기본 상태 설정
    patrolStart = position;
    patrolEnd = position + glm::vec3(50.0f, 0.0f, 50.0f); // 5단위 거리에서 패트롤
    direction = glm::normalize(patrolEnd - patrolStart);
    boundingRadius = 2.f;
}



bool Enemy::IsActive() const {
    return isActive;
}



void Enemy::TakeDamage(int amount, const glm::vec3& bulletDirection) {
    health -= amount;
    if (health <= 0) {
        isActive = false;
    }
    // 뒤로 밀려남
    position += bulletDirection * 2.f; // 총알 방향의 반대로 밀려남
    // 총알에 맞았을 때 점프 시작
    if (!isJumping) {
        isJumping = true;
        jumpSpeed = 3.0f; // 초기 점프 속도
    }
}


void Enemy::Move() {
    //if (isActive) {
        position += direction * moveSpeed;
    //}
}

void Enemy::SetPatrolPoints(const glm::vec3& start, const glm::vec3& end)
{
    patrolStart = start;
    patrolEnd = end;
    direction = glm::normalize(patrolEnd - patrolStart); // 패트롤 기본 방향 설정
}

float Enemy::GetBoundingRadius()
{
    return boundingRadius;
}

bool Enemy::CheckCollisionWithActors(const std::vector<std::unique_ptr<Actor>>& actors, glm::vec3 blockSize)
{
    for (const auto& actor : actors)
    {
        glm::vec3 actorPos = actor->GetPosition();
        if (glm::distance(this->GetPosition(), actorPos) < blockSize.x) {
            direction = -direction;
            return true; // 충돌 발생
        }
    }
    return false; // 충돌 없음
}

glm::vec3 Enemy::GetDirection()
{
    return direction;
}

void Enemy::Patrol(
    const std::vector<std::vector<int>>& mazeMap,
    const glm::vec3& blockSize,
    int mazeX, int mazeZ,
    float deltaTime)
{
    glm::vec3 nextPosition = position + direction * moveSpeed * deltaTime;

    // 다음 위치를 미로 셀 기준으로 변환
    int nextMazeX = static_cast<int>(nextPosition.x / blockSize.x);
    int nextMazeZ = static_cast<int>(nextPosition.z / blockSize.z);


    // 현재 위치를 미로 셀 기준으로 변환
    int currentMazeX = static_cast<int>(position.x / blockSize.x);
    int currentMazeZ = static_cast<int>(position.z / blockSize.z);

    // 이동 가능 여부 확인
    if (isValid(nextMazeX, nextMazeZ, mazeMap)) {
        position = nextPosition;

        //// 패트롤 경로 끝에 도달했는지 확인
        //if (glm::length(position - patrolEnd) < 0.1f) {
        //    std::swap(patrolStart, patrolEnd);
        //    direction = glm::normalize(patrolEnd - patrolStart);
        //}
    }
    else {
        // 이동 불가능하면 방향 반대로 변경
        ChangeDirectionRandomly(currentMazeX, currentMazeZ, mazeMap, blockSize);
    }
}

void Enemy::Chase(
    const glm::vec3& playerPosition,
    const std::vector<std::vector<int>>& mazeMap,
    const glm::vec3& blockSize,
    int mazeX, int mazeZ,
    float deltaTime)
{
    // 플레이어와의 방향 계산
    glm::vec3 targetDirection = glm::normalize(playerPosition - position);

    // 다음 위치 계산
    glm::vec3 nextPosition = position + targetDirection * moveSpeed * deltaTime;

    // 다음 위치를 미로 셀 기준으로 변환
    int nextMazeX = static_cast<int>(nextPosition.x / blockSize.x);
    int nextMazeZ = static_cast<int>(nextPosition.z / blockSize.z);

    // 이동 가능 여부 확인
    if (isValid(nextMazeX, nextMazeZ, mazeMap)) {
        position = nextPosition;
    }
    else {
        // 이동 불가능하면 다시 패트롤 상태로
        currentState = EnemyState::Patrol;
        direction = glm::normalize(patrolEnd - patrolStart);
    }
}

bool Enemy::isValid(int x, int z, const std::vector<std::vector<int>>& mazeMap) {
    return x >= 0 && z >= 0 && x < mazeMap[0].size() && z < mazeMap.size() && mazeMap[z][x] == 0;
}


void Enemy::Update(float deltaTime, 
    const glm::vec3& playerPosition, 
    const std::vector<std::vector<int>>& mazeMap, 
    const glm::vec3& blockSize)
{
    if (!isActive) return;



    // 현재 위치를 미로 셀 기준으로 변환
    int mazeX = static_cast<int>(position.x / blockSize.x);
    int mazeZ = static_cast<int>(position.z / blockSize.z);

    // 중력 및 점프 처리
    if (isJumping) {
        position.y += jumpSpeed * deltaTime;
        jumpSpeed -= gravity * deltaTime;

        // 땅에 도달했을 때
        if (position.y <= 0.0f) {
            position.y = 0.0f;
            isJumping = false;
        }
    }



    float distanceToPlayer = glm::length(playerPosition - position);


    switch (currentState)
    {
    case EnemyState::Patrol:
        if (distanceToPlayer < detectionRadius) {
            currentState = EnemyState::Chase;
        }
        else {
            Patrol(mazeMap, blockSize, mazeX, mazeZ, deltaTime);
        }
        break;

    case EnemyState::Chase:
        Chase(playerPosition, mazeMap, blockSize, mazeX, mazeZ, deltaTime);
        break;
    }
}


glm::vec3 Enemy::FindAlternativeDirection(const glm::vec3& blockPosition, const glm::vec3& blockSize) {
    // 상하좌우 방향
    const std::vector<glm::vec3> directions = {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f)
    };

    for (const auto& dir : directions) {
        glm::vec3 potentialPosition = position + dir * blockSize;
        if (glm::distance(potentialPosition, blockPosition) > blockSize.x) {
            return dir;
        }
    }

    return -direction; // 대체 방향이 없으면 반대 방향으로
}




void Enemy::ChangeDirectionRandomly(int currentMazeX, int currentMazeZ, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize) {
    // 상하좌우 방향
    const std::vector<glm::vec3> directions = {
        glm::vec3(1.0f, 0.0f, 0.0f),  // 오른쪽
        glm::vec3(-1.0f, 0.0f, 0.0f), // 왼쪽
        glm::vec3(0.0f, 0.0f, 1.0f),  // 위쪽
        glm::vec3(0.0f, 0.0f, -1.0f)  // 아래쪽
    };

    std::vector<glm::vec3> validDirections;

    // 유효한 방향 필터링
    for (const auto& dir : directions) {
        int nextX = currentMazeX + static_cast<int>(dir.x);
        int nextZ = currentMazeZ + static_cast<int>(dir.z);

        if (isValid(nextX, nextZ, mazeMap)) {
            validDirections.push_back(dir);
        }
    }

    // 랜덤으로 방향 변경
    if (!validDirections.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, validDirections.size() - 1);

        direction = validDirections[dis(gen)];
    }
    else {
        direction = -direction; // 유효한 방향이 없으면 반대 방향으로
    }
}