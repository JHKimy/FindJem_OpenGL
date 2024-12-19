#include "Enemy.h"

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
    chaseRadius(15.0f),
    homePosition(position)
{
    // �ʱ� ��ġ�� �⺻ ���� ����
    patrolStart = position;
    patrolEnd = position + glm::vec3(3.0f, 0.0f, 0.0f); // 5���� �Ÿ����� ��Ʈ��
    direction = glm::normalize(patrolEnd - patrolStart);
    boundingRadius = 2.f;
}

bool Enemy::IsActive() const {
    return isActive;
}

void Enemy::TakeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        isActive = false;
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


void Enemy::Update(float deltaTime, 
    const glm::vec3& playerPosition, 
    const std::vector<std::vector<int>>& mazeMap, 
    const glm::vec3& blockSize)
{
    if (!isActive) return;

    float distanceToPlayer = glm::length(playerPosition - position);


    switch (currentState)
    {
    case EnemyState::Patrol:
        // ĳ���Ͱ� Ž�� �ݰ� �ȿ� ������ ���� ���·� ��ȯ
        if (distanceToPlayer < detectionRadius)
        {
            currentState = EnemyState::Chase;
        }
        else
        {
            // �̵� ���� ��ġ ���
            glm::vec3 nextPosition = position + direction * moveSpeed * deltaTime;

            // �̷��� �� ���� ���� Ȯ��
            int mazeX = static_cast<int>(nextPosition.x / blockSize.x);
            int mazeZ = static_cast<int>(nextPosition.z / blockSize.z);

            if (mazeX >= 0 && mazeZ >= 0 && mazeX < mazeMap[0].size() && mazeZ < mazeMap.size() && mazeMap[mazeZ][mazeX] == 0)
            {
                // �̵� �����ϸ� ��ġ ������Ʈ
                position = nextPosition;

                // ��Ʈ�� ��� ���� �����ߴ��� Ȯ��
                if (glm::length(position - patrolEnd) < 0.1f)
                {
                    // ��� ���� �����ϸ� ���� �ݴ�� ����
                    glm::vec3 temp = patrolStart;
                    patrolStart = patrolEnd;
                    patrolEnd = temp;
                    direction = glm::normalize(patrolEnd - patrolStart);
                }
            }
            else
            {
                // �̵� �Ұ����ϸ� ���� �ݴ�� ����
                direction = -direction;
            }
        }
        break;

    case EnemyState::Chase:
        // �÷��̾ ���� �̵�
        direction = glm::normalize(playerPosition - position);
        glm::vec3 nextPosition = position + direction * moveSpeed * 1.5f * deltaTime;

        // �̷��� �� ���� ���� Ȯ��
        int mazeX = static_cast<int>(nextPosition.x / blockSize.x);
        int mazeZ = static_cast<int>(nextPosition.z / blockSize.z);

        if (mazeX >= 0 && mazeZ >= 0 && mazeX < mazeMap[0].size() && mazeZ < mazeMap.size() && mazeMap[mazeZ][mazeX] == 0)
        {
            position = nextPosition;
        }
        else
        {
            // ���� �ε��� ��� �ٽ� ��Ʈ�ѷ� ����
            currentState = EnemyState::Patrol;
            direction = glm::normalize(patrolEnd - patrolStart);
        }
        break;
    }
}