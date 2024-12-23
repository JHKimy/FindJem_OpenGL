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
    chaseRadius(20.0f),
    homePosition(position),
    isJumping(false),
    jumpSpeed(0.0f)
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


void Enemy::Update(float deltaTime, 
    const glm::vec3& playerPosition, 
    const std::vector<std::vector<int>>& mazeMap, 
    const glm::vec3& blockSize)
{
    if (!isActive) return;


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