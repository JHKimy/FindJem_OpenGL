#pragma once
#include <queue>
#include <unordered_map>
#include <vector>
#include <utility>
#include "Types.h"

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
	static std::vector<vec2> FindPath(
		const vec2 & start,
		const vec2 & goal,
		const std::vector<std::vector<int>>& mazeMap);

private:
	static float CalculateHeuristic(const vec2& current, const vec2& goal);
};

