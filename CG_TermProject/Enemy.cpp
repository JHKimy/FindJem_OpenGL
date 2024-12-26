#include "Enemy.h"
#include <random>
#include <queue>
#include <iostream> // for cout

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
	jumpSpeed(0.0f),
	currentPathIndex(0)
{
	boundingRadius = 2.f;
}

void Enemy::InitializePatrolPath(const std::vector<std::vector<int>>& mazeMap, const glm::vec3& blockSize)
{
	patrolPath = CalculatePatrolPath(homePosition, mazeMap, blockSize);
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
			Patrol(mazeMap, blockSize, deltaTime);
		}
		break;

	case EnemyState::Chase:
		if (distanceToPlayer > detectionRadius)
		{
			Patrol(mazeMap, blockSize, deltaTime);
		}
		else {
			Chase(playerPosition, mazeMap, blockSize, deltaTime);
		}
		break;
	}
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



void Enemy::Patrol(
	const std::vector<std::vector<int>>& mazeMap,
	const glm::vec3& blockSize,
	float deltaTime)
{
	// 현재 위치를 미로 좌표로 변환
	int mazeX = static_cast<int>(position.x / blockSize.x);
	int mazeZ = static_cast<int>(position.z / blockSize.z);

	// 이동 가능한 방향 (x축과 z축의 한 방향만 포함)
	const std::vector<glm::ivec2> directions =
	{
		{1, 0}, {-1, 0}, // x축: 오른쪽, 왼쪽
		{0, 1}, {0, -1}  // z축: 위쪽, 아래쪽
	};

	// 현재 이동 방향 및 목표 타일 저장
	static glm::ivec2 currentDir = directions[0];        // 초기 방향 설정
	static glm::ivec2 targetTile = { mazeX, mazeZ };       // 초기 목표 타일 설정

	// 목표 타일의 월드 좌표 계산
	glm::vec3 targetPosition = glm::vec3(
		targetTile.x * blockSize.x,
		position.y,
		targetTile.y * blockSize.z
	);

	// 목표 타일에 도달했는지 확인
	if (glm::length(targetPosition - position) < 0.1f)
	{
		// 현재 목표 타일에 도달했으므로 새로운 타일로 이동 방향 설정
		bool foundNewDirection = false;

		for (const auto& dir : directions)
		{
			int newMazeX = mazeX + dir.x;
			int newMazeZ = mazeZ + dir.y;

			// 유효한 방향 중 현재 방향의 반대 방향은 제외
			if (isValid(newMazeX, newMazeZ, mazeMap) && dir != -currentDir)
			{
				currentDir = dir;
				targetTile = { newMazeX, newMazeZ };
				foundNewDirection = true;
				break;
			}
		}

		// 모든 방향이 막혀 있다면 반대 방향으로 돌아감
		if (!foundNewDirection)
		{
			currentDir = -currentDir;
			targetTile = { mazeX + currentDir.x, mazeZ + currentDir.y };
		}

		// 새로운 목표 타일의 월드 좌표 계산
		targetPosition = glm::vec3(
			targetTile.x * blockSize.x,
			position.y,
			targetTile.y * blockSize.z
		);
	}

	// 현재 위치에서 목표 위치로 자연스럽게 이동
	glm::vec3 directionToTarget = glm::normalize(targetPosition - position);
	position += directionToTarget * moveSpeed * deltaTime;

	// 목표 위치 초과 방지
	if (glm::length(targetPosition - position) < 0.1f)
	{
		position = targetPosition;
	}

	// 디버그 정보 출력
	std::cout << "Current Position: (" << position.x << ", " << position.z << ")\n";
	std::cout << "Target Tile: (" << targetTile.x << ", " << targetTile.y << ")\n";
	std::cout << "Current Direction: (" << currentDir.x << ", " << currentDir.y << ")\n";
}







void Enemy::Chase(
	const glm::vec3& playerPosition,
	const std::vector<std::vector<int>>& mazeMap,
	const glm::vec3& blockSize,
	float deltaTime)
{
	direction = glm::normalize(playerPosition - position);
	position += direction * moveSpeed * deltaTime;

	/*glm::vec3 targetDirection = glm::normalize(playerPosition - position);
	glm::vec3 nextPosition = position + targetDirection * moveSpeed * deltaTime;

	int nextMazeX = static_cast<int>(nextPosition.x / blockSize.x);
	int nextMazeZ = static_cast<int>(nextPosition.z / blockSize.z);

	if (isValid(nextMazeX, nextMazeZ, mazeMap)) {
		position = nextPosition;
	}
	else {
		currentState = EnemyState::Patrol;
		currentPathIndex = 0;
	}*/
}

std::vector<glm::vec3> Enemy::CalculatePatrolPath(
	const glm::vec3& startPosition,
	const std::vector<std::vector<int>>& mazeMap,
	const glm::vec3& blockSize)
{
	std::vector<glm::vec3> path;
	std::queue<glm::ivec2> queue;
	std::vector<std::vector<bool>> visited(mazeMap.size(), std::vector<bool>(mazeMap[0].size(), false));

	int startX = static_cast<int>(startPosition.x / blockSize.x);
	int startZ = static_cast<int>(startPosition.z / blockSize.z);

	queue.push({ startX, startZ });
	visited[startZ][startX] = true;

	const vector<glm::ivec2> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

	while (!queue.empty()) {
		glm::ivec2 current = queue.front();
		queue.pop();

		glm::vec3 worldPosition = glm::vec3(current.x * blockSize.x, 0.0f, current.y * blockSize.z);
		path.push_back(worldPosition);

		for (const auto& dir : directions)
		{
			int nextX = current.x + dir.x;
			int nextZ = current.y + dir.y;

			if (nextX >= 0 && nextZ >= 0 && nextX < mazeMap[0].size() && nextZ < mazeMap.size() &&
				mazeMap[nextZ][nextX] == 0 && !visited[nextZ][nextX])
			{
				queue.push({ nextX, nextZ });
				visited[nextZ][nextX] = true;
			}
		}
	}

	return path;
}



// 시작과 끝 확인
bool Enemy::isValid(int x, int z, const std::vector<std::vector<int>>& mazeMap) {
	return x >= 0 && z >= 0 &&
		x < mazeMap[0].size() && z < mazeMap.size() &&
		mazeMap[z][x] == 0;
}


