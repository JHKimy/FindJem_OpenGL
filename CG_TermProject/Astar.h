#pragma once
#include "OpenGL/glm/glm.hpp"
#include <queue>
#include <unordered_map>
#include <vector>

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

	static std::vector<glm::ivec2> FindPath(
		const glm::ivec2& start,
		const glm::ivec2& goal,
		const std::vector<std::vector<int>>& mazeMap);

private:
	static float CalculateHeuristic(const glm::ivec2& current, const glm::ivec2& goal);
};

