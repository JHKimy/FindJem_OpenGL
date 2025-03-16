#include "AStarPath.h"
#include <iostream>


std::vector<vec2> Astar::FindPath(const vec2& start, const vec2& goal, const std::vector<std::vector<int>>& mazeMap)
{
	// 2D �̷� ��ǥ�� �ε����� ��ȯ �Լ�
	auto getIndex = [&mazeMap](int x, int z) {
		return z * mazeMap[0].size() + x;
		};

	// Ž���� ��� �켱 ���� ť : ���� ���� ���� �� ���� (�ּ� ��)
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
	// �̹� Ž���� ��� ���� ��
	std::unordered_map<int, Node> closedList;

	// Ž�� �� ���� : �����¿� (���� ���⸸ ���)
	std::vector<vec2> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

	// =======

	// 1. ���� ��� ���� �� ���¸���Ʈ�� �ֱ�
	Node startNode{ start.x, start.z, 0, CalculateHeuristic(start, goal), nullptr };
	openList.push(startNode);

	// 2. ���¸���Ʈ�� �������� Ž��
	while (!openList.empty())
	{
		// 2-1. 
		// ��� ���� ���� ��� �������� 
		// �ε��� ���ϰ� ����
		Node current = openList.top();
		openList.pop();
		int currentIndex = getIndex(current.x, current.z);

		// 2-2. �ε����� �̿��� �̹� �湮�� Ŭ����帮��Ʈ�� ������ �ǳʶٱ�
		if (closedList.find(currentIndex) != closedList.end()) continue;

		// 2-2. �ƴϸ� �̹� Ž���� ��忡 �־��ֱ�
		closedList[currentIndex] = current;


		// 4. ���ǿ� ������ ��� ���
		// ���� ��尡 ��ǥ��ġ�� ��ġ�ϸ� ���� ��� �����
		if (current.x == goal.x && current.z == goal.z) 
		{
			// 4-1. Ŭ����� ����Ʈ���ִ� ��带 �ϳ��� ������ path�� ���
			std::vector<vec2> path;
			Node* pathNode = &closedList[currentIndex];
			while (pathNode) 
			{
				path.emplace_back(pathNode->x, pathNode->z);
				// 4-2. ���� ��带 �θ� ���� �ٲٱ�
				pathNode = pathNode->parent;
			}
			// 4-3. ������� ��µǰ� �ٲ��ֱ�
			std::reverse(path.begin(), path.end());
			
			// 4-4. ���
			return path;
		}

		// 3. ���ο� ��� �߰�
		// �����¿� ���� (�밢���� �ȵ�)
		for (const auto& dir : directions)
		{
			int nextX = current.x + dir.x;
			int nextZ = current.z + dir.z;

			// 3-1. ������ ����� �ʰ�, ���� �ƴ� ���� ����
			if (nextX >= 0 && nextZ >= 0 && nextX < mazeMap[0].size() && nextZ < mazeMap.size() && mazeMap[nextZ][nextX] == 0) {

				// 3-2. ���� ��尡 �̹� �湮�� ����� �ǳʶ�
				int nextIndex = getIndex(nextX, nextZ);
				if (closedList.find(nextIndex) != closedList.end()) continue;

				// 3-3. ��ĭ �̵� �Ҷ� ���� ��� 1 �߰�
				float nextCost = current.cost + 1;

				// 3-4. ���ο� ��带 ���� �� ���¸���Ʈ�� �߰�
				Node nextNode{ nextX, nextZ, nextCost, CalculateHeuristic({(float)nextX, (float)nextZ}, goal), &closedList[currentIndex] };
				openList.push(nextNode);
			}
		}
	}

	return {}; // ��θ� ã�� �� ���� ��� �� ���� ��ȯ

}

// ����ư �Ÿ��� ���
float Astar::CalculateHeuristic(const vec2& current, const vec2& goal)
{
	return abs(current.x - goal.x) + abs(current.z - goal.z);

}
