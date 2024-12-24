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
    // �ʱ� ��ġ�� �⺻ ���� ����
    patrolStart = position;
    patrolEnd = position + glm::vec3(50.0f, 0.0f, 50.0f); // 5���� �Ÿ����� ��Ʈ��
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
    // �ڷ� �з���
    position += bulletDirection * 2.f; // �Ѿ� ������ �ݴ�� �з���
    // �Ѿ˿� �¾��� �� ���� ����
    if (!isJumping) {
        isJumping = true;
        jumpSpeed = 3.0f; // �ʱ� ���� �ӵ�
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
    direction = glm::normalize(patrolEnd - patrolStart); // ��Ʈ�� �⺻ ���� ����
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
            return true; // �浹 �߻�
        }
    }
    return false; // �浹 ����
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

    // ���� ��ġ�� �̷� �� �������� ��ȯ
    int nextMazeX = static_cast<int>(nextPosition.x / blockSize.x);
    int nextMazeZ = static_cast<int>(nextPosition.z / blockSize.z);


    // ���� ��ġ�� �̷� �� �������� ��ȯ
    int currentMazeX = static_cast<int>(position.x / blockSize.x);
    int currentMazeZ = static_cast<int>(position.z / blockSize.z);

    // �̵� ���� ���� Ȯ��
    if (isValid(nextMazeX, nextMazeZ, mazeMap)) {
        position = nextPosition;

        //// ��Ʈ�� ��� ���� �����ߴ��� Ȯ��
        //if (glm::length(position - patrolEnd) < 0.1f) {
        //    std::swap(patrolStart, patrolEnd);
        //    direction = glm::normalize(patrolEnd - patrolStart);
        //}
    }
    else {
        // �̵� �Ұ����ϸ� ���� �ݴ�� ����
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
    // �÷��̾���� ���� ���
    glm::vec3 targetDirection = glm::normalize(playerPosition - position);

    // ���� ��ġ ���
    glm::vec3 nextPosition = position + targetDirection * moveSpeed * deltaTime;

    // ���� ��ġ�� �̷� �� �������� ��ȯ
    int nextMazeX = static_cast<int>(nextPosition.x / blockSize.x);
    int nextMazeZ = static_cast<int>(nextPosition.z / blockSize.z);

    // �̵� ���� ���� Ȯ��
    if (isValid(nextMazeX, nextMazeZ, mazeMap)) {
        position = nextPosition;
    }
    else {
        // �̵� �Ұ����ϸ� �ٽ� ��Ʈ�� ���·�
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



    // ���� ��ġ�� �̷� �� �������� ��ȯ
    int mazeX = static_cast<int>(position.x / blockSize.x);
    int mazeZ = static_cast<int>(position.z / blockSize.z);

    // �߷� �� ���� ó��
    if (isJumping) {
        position.y += jumpSpeed * deltaTime;
        jumpSpeed -= gravity * deltaTime;

        // ���� �������� ��
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
    // �����¿� ����
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

    return -direction; // ��ü ������ ������ �ݴ� ��������
}




void Enemy::ChangeDirectionRandomly(int currentMazeX, int currentMazeZ, const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize) {
    // �����¿� ����
    const std::vector<glm::vec3> directions = {
        glm::vec3(1.0f, 0.0f, 0.0f),  // ������
        glm::vec3(-1.0f, 0.0f, 0.0f), // ����
        glm::vec3(0.0f, 0.0f, 1.0f),  // ����
        glm::vec3(0.0f, 0.0f, -1.0f)  // �Ʒ���
    };

    std::vector<glm::vec3> validDirections;

    // ��ȿ�� ���� ���͸�
    for (const auto& dir : directions) {
        int nextX = currentMazeX + static_cast<int>(dir.x);
        int nextZ = currentMazeZ + static_cast<int>(dir.z);

        if (isValid(nextX, nextZ, mazeMap)) {
            validDirections.push_back(dir);
        }
    }

    // �������� ���� ����
    if (!validDirections.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, validDirections.size() - 1);

        direction = validDirections[dis(gen)];
    }
    else {
        direction = -direction; // ��ȿ�� ������ ������ �ݴ� ��������
    }
}