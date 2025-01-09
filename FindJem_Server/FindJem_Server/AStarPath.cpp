#include "AStarPath.h"
#include <iostream>

std::vector<std::pair<int, int>> Astar::FindPath(const std::pair<int, int>& start, const std::pair<int, int>& goal, const std::vector<std::vector<int>>& mazeMap)
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
	Node startNode{ start.first, start.second, 0, CalculateHeuristic(start, goal), nullptr };
	openList.push(startNode);

	// ���� ���⸸ ���
	std::vector<std::pair<int,int>> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

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
		if (current.x == goal.first && current.z == goal.second) {
			std::vector<std::pair<int,int>> path;
			Node* pathNode = &closedList[currentIndex];
			while (pathNode) {
				path.emplace_back(pathNode->x, pathNode->z);
				pathNode = pathNode->parent;
			}
			std::reverse(path.begin(), path.end());

			// ���� ��� ���
			std::cout << "Predicted Path: ";
			for (const auto& tile : path) {
				std::cout << "(" << tile.first << ", " << tile.second << ") ";
			}
			std::cout << std::endl;

			return path;
		}

		for (const auto& dir : directions) {
			int nextX = current.x + dir.first;
			int nextZ = current.z + dir.second;

			// ������ ����� �ʰ�, ���� �ƴϸ�, �밢���� ����
			if (nextX >= 0 && nextZ >= 0 && nextX < mazeMap[0].size() && nextZ < mazeMap.size() && mazeMap[nextZ][nextX] == 0) {
				// �� ���̸� ������� �ʵ��� üũ
				if (dir.first != 0 && dir.second != 0) {
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

float Astar::CalculateHeuristic(const std::pair<int, int>& current, const std::pair<int, int>& goal)
{
	return abs(current.first - goal.first) + abs(current.second - goal.second);

}
