#include "Astar.h"
#include <iostream>

std::vector<glm::ivec2> Astar::FindPath(const glm::ivec2& start, const glm::ivec2& goal, const std::vector<std::vector<int>>& mazeMap)
{
	// Ž���� ��� -> �ּ��� : ���� ���� ���� �� ����
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
	// �̹� Ž���� ���
	std::unordered_map<int, Node> closedList;

	// 2D �̷� ��ǥ �ε����� ��ȯ
	auto getIndex = [&mazeMap](int x, int z) {
		return z * mazeMap[0].size() + x;
		};


	// ���� ��� (����ư �Ÿ��� ����Ÿ� ���)
	Node startNode{ start.x, start.y, 0, CalculateHeuristic(start, goal), nullptr };
	openList.push(startNode);

	// ���� ���⸸ ���
	std::vector<glm::ivec2> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

	while (!openList.empty()) 
	{
		// ��� ���� ���� ��� �������� ����
		Node current = openList.top();
		openList.pop();
		// ��� ���� ���� ��� �ε���
		int currentIndex = getIndex(current.x, current.z);
		
		// �̹� �湮�� ���� �ǳʶٱ�
		if (closedList.find(currentIndex) != closedList.end()) continue;

		// �̹� Ž���� ��忡 �־��ֱ�
		closedList[currentIndex] = current;

		// ���� ��尡 ��ǥ��ġ�� ��ġ�ϸ� ��� ����
		if (current.x == goal.x && current.z == goal.y) {
			std::vector<glm::ivec2> path;
			Node* pathNode = &closedList[currentIndex];
			while (pathNode) {
				path.emplace_back(pathNode->x, pathNode->z);
				pathNode = pathNode->parent;
			}
			std::reverse(path.begin(), path.end());

			// ���� ��� ���
			std::cout << "Predicted Path: ";
			for (const auto& tile : path) {
				std::cout << "(" << tile.x << ", " << tile.y << ") ";
			}
			std::cout << std::endl;

			return path;
		}

		for (const auto& dir : directions) {
			int nextX = current.x + dir.x;
			int nextZ = current.z + dir.y;

			// ������ ����� �ʰ�, ���� �ƴϸ�, �밢���� ����
			if (nextX >= 0 && nextZ >= 0 && nextX < mazeMap[0].size() && nextZ < mazeMap.size() && mazeMap[nextZ][nextX] == 0) {
				// �� ���̸� ������� �ʵ��� üũ
				if (dir.x != 0 && dir.y != 0) {
					if (mazeMap[current.z][nextX] == 1 || mazeMap[nextZ][current.x] == 1) {
						continue; // �밢�� �̵� ����
					}
				}

				int nextIndex = getIndex(nextX, nextZ);
				if (closedList.find(nextIndex) != closedList.end()) continue;

				float nextCost = current.cost + 1;
				Node nextNode{ nextX, nextZ, nextCost, CalculateHeuristic({nextX, nextZ}, goal), &closedList[currentIndex] };
				openList.push(nextNode);
			}
		}
	}

	return {}; // ��θ� ã�� �� ���� ��� �� ���� ��ȯ

}

float Astar::CalculateHeuristic(const glm::ivec2& current, const glm::ivec2& goal)
{
	return abs(current.x - goal.x) + abs(current.y - goal.y);
}
