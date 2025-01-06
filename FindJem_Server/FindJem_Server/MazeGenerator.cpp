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

	// ���� ��
	int startX = 1, startY = 1;
	mazeMap[startX][startY] = 0;

	// �����¿� ����
	const vector<pair<int, int>> directions = { {0,-2},{0,2},{-2,0},{2,0} };

	// �����¿� ����
	for (const auto& dir : directions)
	{
		// ���� ��ǥ�� ����
		int nx = startX + dir.first;
		int ny = startY + dir.second;

		if (isValid(nx, ny))
		{
			// ���� ���� �����¿� ��ǥ�� �־��ֱ�
			edgeQueue.push({ startX, startY, nx, ny, rand() % 100 });
		}
	}

	while (!edgeQueue.empty())
	{
		Edge current = edgeQueue.top();
		edgeQueue.pop();

		// ���� ��ġ�� ���� �ƴϸ� �� �W��
		if (isValid(current.nx, current.ny)
			&& mazeMap[current.ny][current.nx] == 1)
		{
			mazeMap[current.y][current.x] = 0;		// ���� ��ġ�� ���
			mazeMap[current.ny][current.nx] = 0;	// ���� ��ġ ���
			mazeMap[(current.y + current.ny) / 2][(current.x + current.nx) / 2] = 0;	// �߰� �� ����
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
	// �»�� (0, 1)�� �Ա��� ����
	mazeMap[1][0] = 0;
	mazeMap[1][1] = 0;

	// ���ϴ� (width-2, height-1)�� �ⱸ�� ����
	mazeMap[height - 2][width - 1] = 0;
	mazeMap[height - 2][width - 2] = 0;
}