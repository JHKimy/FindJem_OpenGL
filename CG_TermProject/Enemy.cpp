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
    // 초기 위치와 기본 상태 설정
    patrolStart = position;
    patrolEnd = position + glm::vec3(3.0f, 0.0f, 0.0f); // 5단위 거리에서 패트롤
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


void Enemy::Update(float deltaTime, 
    const glm::vec3& playerPosition, 
    const std::vector<std::vector<int>>& mazeMap, 
    const glm::vec3& blockSize)
{
    if (!isActive) return;


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
        // 캐릭터가 탐지 반경 안에 들어오면 추적 상태로 전환
        if (distanceToPlayer < detectionRadius)
        {
            currentState = EnemyState::Chase;
        }
        else
        {
            // 이동 예정 위치 계산
            glm::vec3 nextPosition = position + direction * moveSpeed * deltaTime;

            // 미로의 빈 공간 여부 확인
            int mazeX = static_cast<int>(nextPosition.x / blockSize.x);
            int mazeZ = static_cast<int>(nextPosition.z / blockSize.z);

            if (mazeX >= 0 && mazeZ >= 0 && mazeX < mazeMap[0].size() && mazeZ < mazeMap.size() && mazeMap[mazeZ][mazeX] == 0)
            {
                // 이동 가능하면 위치 업데이트
                position = nextPosition;

                // 패트롤 경로 끝에 도달했는지 확인
                if (glm::length(position - patrolEnd) < 0.1f)
                {
                    // 경로 끝에 도달하면 방향 반대로 설정
                    glm::vec3 temp = patrolStart;
                    patrolStart = patrolEnd;
                    patrolEnd = temp;
                    direction = glm::normalize(patrolEnd - patrolStart);
                }
            }
            else
            {
                // 이동 불가능하면 방향 반대로 변경
                direction = -direction;
            }
        }
        break;

    case EnemyState::Chase:
        // 플레이어를 향해 이동
        direction = glm::normalize(playerPosition - position);
        glm::vec3 nextPosition = position + direction * moveSpeed * 1.5f * deltaTime;

        // 미로의 빈 공간 여부 확인
        int mazeX = static_cast<int>(nextPosition.x / blockSize.x);
        int mazeZ = static_cast<int>(nextPosition.z / blockSize.z);

        if (mazeX >= 0 && mazeZ >= 0 && mazeX < mazeMap[0].size() && mazeZ < mazeMap.size() && mazeMap[mazeZ][mazeX] == 0)
        {
            position = nextPosition;
        }
        else
        {
            // 벽에 부딪힌 경우 다시 패트롤로 복귀
            currentState = EnemyState::Patrol;
            direction = glm::normalize(patrolEnd - patrolStart);
        }
        break;
    }
}