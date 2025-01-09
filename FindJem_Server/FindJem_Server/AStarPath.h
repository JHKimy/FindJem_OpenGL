#pragma once
#include <queue>
#include <unordered_map>
#include <vector>
#include <utility>

class Astar
{
public:
	struct Node
	{
		int x, z;
		float cost;
		float heuristic;
		Node* parent;

		float TotalCost() const {
			return cost + heuristic;
		}

		bool operator>(const Node& other) const {
			return TotalCost() > other.TotalCost();
		}
	};


	// 경로 찾기
	static std::vector<std::pair<int,int>> FindPath(
		const std::pair<int, int> & start,
		const std::pair<int, int> & goal,
		const std::vector<std::vector<int>>& mazeMap);

private:
	static float CalculateHeuristic(const std::pair<int, int>& current, const std::pair<int, int>& goal);
};

