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
	// �ڷ� �з���
	position += bulletDirection * 2.f; // �Ѿ� ������ �ݴ�� �з���
	// �Ѿ˿� �¾��� �� ���� ����
	if (!isJumping) {
		isJumping = true;
		jumpSpeed = 3.0f; // �ʱ� ���� �ӵ�
	}
}



void Enemy::Patrol(
	const std::vector<std::vector<int>>& mazeMap,
	const glm::vec3& blockSize,
	float deltaTime)
{
	// ���� ��ġ�� �̷� ��ǥ�� ��ȯ
	int mazeX = static_cast<int>(position.x / blockSize.x);
	int mazeZ = static_cast<int>(position.z / blockSize.z);

	// �̵� ������ ���� (x��� z���� �� ���⸸ ����)
	const std::vector<glm::ivec2> directions =
	{
		{1, 0}, {-1, 0}, // x��: ������, ����
		{0, 1}, {0, -1}  // z��: ����, �Ʒ���
	};

	// ���� �̵� ���� �� ��ǥ Ÿ�� ����
	static glm::ivec2 currentDir = directions[0];        // �ʱ� ���� ����
	static glm::ivec2 targetTile = { mazeX, mazeZ };       // �ʱ� ��ǥ Ÿ�� ����

	// ��ǥ Ÿ���� ���� ��ǥ ���
	glm::vec3 targetPosition = glm::vec3(
		targetTile.x * blockSize.x,
		position.y,
		targetTile.y * blockSize.z
	);

	// ��ǥ Ÿ�Ͽ� �����ߴ��� Ȯ��
	if (glm::length(targetPosition - position) < 0.1f)
	{
		// ���� ��ǥ Ÿ�Ͽ� ���������Ƿ� ���ο� Ÿ�Ϸ� �̵� ���� ����
		bool foundNewDirection = false;

		for (const auto& dir : directions)
		{
			int newMazeX = mazeX + dir.x;
			int newMazeZ = mazeZ + dir.y;

			// ��ȿ�� ���� �� ���� ������ �ݴ� ������ ����
			if (isValid(newMazeX, newMazeZ, mazeMap) && dir != -currentDir)
			{
				currentDir = dir;
				targetTile = { newMazeX, newMazeZ };
				foundNewDirection = true;
				break;
			}
		}

		// ��� ������ ���� �ִٸ� �ݴ� �������� ���ư�
		if (!foundNewDirection)
		{
			currentDir = -currentDir;
			targetTile = { mazeX + currentDir.x, mazeZ + currentDir.y };
		}

		// ���ο� ��ǥ Ÿ���� ���� ��ǥ ���
		targetPosition = glm::vec3(
			targetTile.x * blockSize.x,
			position.y,
			targetTile.y * blockSize.z
		);
	}

	// ���� ��ġ���� ��ǥ ��ġ�� �ڿ������� �̵�
	glm::vec3 directionToTarget = glm::normalize(targetPosition - position);
	position += directionToTarget * moveSpeed * deltaTime;

	// ��ǥ ��ġ �ʰ� ����
	if (glm::length(targetPosition - position) < 0.1f)
	{
		position = targetPosition;
	}

	// ����� ���� ���
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



// ���۰� �� Ȯ��
bool Enemy::isValid(int x, int z, const std::vector<std::vector<int>>& mazeMap) {
	return x >= 0 && z >= 0 &&
		x < mazeMap[0].size() && z < mazeMap.size() &&
		mazeMap[z][x] == 0;
}


