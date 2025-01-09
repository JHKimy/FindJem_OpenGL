#include "AStarPath.h"
#include <iostream>

std::vector<std::pair<int, int>> Astar::FindPath(const std::pair<int, int>& start, const std::pair<int, int>& goal, const std::vector<std::vector<int>>& mazeMap)
{
	// 탐색할 노드 -> 최소힙 : 가장 작은 값이 맨 위로
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
	// 이미 탐색한 노드
	std::unordered_map<int, Node> closedList;

	// 2D 미로 좌표 인덱스로 변환
	auto getIndex = [&mazeMap](int x, int z) {
		return z * mazeMap[0].size() + x;
		};


	// 시작 노드 (맨해튼 거리로 예상거리 계산)
	Node startNode{ start.first, start.second, 0, CalculateHeuristic(start, goal), nullptr };
	openList.push(startNode);

	// 직선 방향만 허용
	std::vector<std::pair<int,int>> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

	while (!openList.empty())
	{
		// 비용 가장 낮은 노드 가져오고 삭제
		Node current = openList.top();
		openList.pop();
		// 비용 가장 낮은 노드 인덱스
		int currentIndex = getIndex(current.x, current.z);

		// 이미 방문한 노드면 건너뛰기
		if (closedList.find(currentIndex) != closedList.end()) continue;

		// 이미 탐색한 노드에 넣어주기
		closedList[currentIndex] = current;

		// 현재 노드가 목표위치와 일치하면 경로 복원
		if (current.x == goal.first && current.z == goal.second) {
			std::vector<std::pair<int,int>> path;
			Node* pathNode = &closedList[currentIndex];
			while (pathNode) {
				path.emplace_back(pathNode->x, pathNode->z);
				pathNode = pathNode->parent;
			}
			std::reverse(path.begin(), path.end());

			// 예상 경로 출력
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

			// 범위를 벗어나지 않고, 벽이 아니며, 대각선을 방지
			if (nextX >= 0 && nextZ >= 0 && nextX < mazeMap[0].size() && nextZ < mazeMap.size() && mazeMap[nextZ][nextX] == 0) {
				// 벽 사이를 통과하지 않도록 체크
				if (dir.first != 0 && dir.second != 0) {
					if (mazeMap[current.z][nextX] == 1 || mazeMap[nextZ][current.x] == 1) {
						continue; // 대각선 이동 방지
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

	return {}; // 경로를 찾을 수 없는 경우 빈 벡터 반환

}

float Astar::CalculateHeuristic(const std::pair<int, int>& current, const std::pair<int, int>& goal)
{
	return abs(current.first - goal.first) + abs(current.second - goal.second);

}
