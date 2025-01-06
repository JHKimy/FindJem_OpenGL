#include "pch.h"
#include "MazeGenerator.h"



using std::priority_queue;
using std::pair;

MazeGenerator::MazeGenerator(int width, int height)
	: width(width), height(height)
{
	srand(static_cast<unsigned int>(time(0)));
	InitializeMaze();
}

void MazeGenerator::InitializeMaze()
{
	mazeMap.assign(height, vector<int>(width, 1));
}

void MazeGenerator::GeneratePrimMaze()
{
	priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> edgeQueue;

	// 시작 점
	int startX = 1, startY = 1;
	mazeMap[startX][startY] = 0;

	// 상하좌우 방향
	const vector<pair<int, int>> directions = { {0,-2},{0,2},{-2,0},{2,0} };

	// 상하좌우 전부
	for (const auto& dir : directions)
	{
		// 다음 좌표에 설정
		int nx = startX + dir.first;
		int ny = startY + dir.second;

		if (isValid(nx, ny))
		{
			// 현재 점과 상하좌우 좌표들 넣어주기
			edgeQueue.push({ startX, startY, nx, ny, rand() % 100 });
		}
	}

	while (!edgeQueue.empty())
	{
		Edge current = edgeQueue.top();
		edgeQueue.pop();

		// 다음 위치가 길이 아니면 길 둟기
		if (isValid(current.nx, current.ny)
			&& mazeMap[current.ny][current.nx] == 1)
		{
			mazeMap[current.y][current.x] = 0;		// 현재 위치를 길로
			mazeMap[current.ny][current.nx] = 0;	// 다음 위치 길로
			mazeMap[(current.y + current.ny) / 2][(current.x + current.nx) / 2] = 0;	// 중간 벽 제거
		}

		for (const auto& dir : directions)
		{
			int nnx = current.nx + dir.first;
			int nny = current.ny + dir.second;
			if (isValid(nnx, nny) && mazeMap[nny][nnx] == 1)
			{
				edgeQueue.push({ current.nx, current.ny, nnx, nny, rand() % 100 });
			}
		}

	}

}

const vector<vector<int>>& MazeGenerator::GetMaze()
{
	return mazeMap;
}

void MazeGenerator::addEntrances()
{
	// 좌상단 (0, 1)을 입구로 설정
	mazeMap[1][0] = 0;
	mazeMap[1][1] = 0;

	// 우하단 (width-2, height-1)을 출구로 설정
	mazeMap[height - 2][width - 1] = 0;
	mazeMap[height - 2][width - 2] = 0;
}