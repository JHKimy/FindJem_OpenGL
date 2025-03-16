#include "AStarPath.h"
#include <iostream>


std::vector<vec2> Astar::FindPath(const vec2& start, const vec2& goal, const std::vector<std::vector<int>>& mazeMap)
{
	// 2D 미로 좌표를 인덱스로 변환 함수
	auto getIndex = [&mazeMap](int x, int z) {
		return z * mazeMap[0].size() + x;
		};

	// 탐색할 노드 우선 순위 큐 : 가장 작은 값이 맨 위로 (최소 힙)
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
	// 이미 탐색한 노드 저장 맵
	std::unordered_map<int, Node> closedList;

	// 탐색 할 방향 : 상하좌우 (직선 방향만 허용)
	std::vector<vec2> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

	// =======

	// 1. 시작 노드 생성 후 오픈리스트에 넣기
	Node startNode{ start.x, start.z, 0, CalculateHeuristic(start, goal), nullptr };
	openList.push(startNode);

	// 2. 오픈리스트가 빌때까지 탐색
	while (!openList.empty())
	{
		// 2-1. 
		// 비용 가장 낮은 노드 가져오고 
		// 인덱스 구하고 삭제
		Node current = openList.top();
		openList.pop();
		int currentIndex = getIndex(current.x, current.z);

		// 2-2. 인덱스를 이용해 이미 방문한 클로즈드리스트에 있으면 건너뛰기
		if (closedList.find(currentIndex) != closedList.end()) continue;

		// 2-2. 아니면 이미 탐색한 노드에 넣어주기
		closedList[currentIndex] = current;


		// 4. 조건에 맞으면 경로 출력
		// 현재 노드가 목표위치와 일치하면 최종 경로 만들기
		if (current.x == goal.x && current.z == goal.z) 
		{
			// 4-1. 클로즈드 리스트에있는 노드를 하나씩 꺼내와 path에 담기
			std::vector<vec2> path;
			Node* pathNode = &closedList[currentIndex];
			while (pathNode) 
			{
				path.emplace_back(pathNode->x, pathNode->z);
				// 4-2. 현재 노드를 부모 노드로 바꾸기
				pathNode = pathNode->parent;
			}
			// 4-3. 순서대로 출력되게 바꿔주기
			std::reverse(path.begin(), path.end());
			
			// 4-4. 출력
			return path;
		}

		// 3. 새로운 경로 추가
		// 상하좌우 방향 (대각선은 안됨)
		for (const auto& dir : directions)
		{
			int nextX = current.x + dir.x;
			int nextZ = current.z + dir.z;

			// 3-1. 범위를 벗어나지 않고, 벽이 아닌 곳만 가능
			if (nextX >= 0 && nextZ >= 0 && nextX < mazeMap[0].size() && nextZ < mazeMap.size() && mazeMap[nextZ][nextX] == 0) {

				// 3-2. 다음 노드가 이미 방문한 노드라면 건너뜀
				int nextIndex = getIndex(nextX, nextZ);
				if (closedList.find(nextIndex) != closedList.end()) continue;

				// 3-3. 한칸 이동 할때 마다 비용 1 추가
				float nextCost = current.cost + 1;

				// 3-4. 새로운 노드를 생성 후 오픈리스트에 추가
				Node nextNode{ nextX, nextZ, nextCost, CalculateHeuristic({(float)nextX, (float)nextZ}, goal), &closedList[currentIndex] };
				openList.push(nextNode);
			}
		}
	}

	return {}; // 경로를 찾을 수 없는 경우 빈 벡터 반환

}

// 맨해튼 거리로 계산
float Astar::CalculateHeuristic(const vec2& current, const vec2& goal)
{
	return abs(current.x - goal.x) + abs(current.z - goal.z);

}
