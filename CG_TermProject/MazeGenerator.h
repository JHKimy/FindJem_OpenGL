//#pragma once
//#include <vector>
//
//using std::vector;
//
//class MazeGenerator
//{
//public:
//	MazeGenerator(int width, int height);
//
//	// ���� �˰������� �̷� ����
//	void GeneratePrimMaze();
//
//	const vector<vector<int>>& GetMaze();
//
//	void addEntrances();
//
//private:
//	struct Edge 
//	{
//		int x, y, nx, ny, weight;
//
//		bool operator>(const Edge& other) const
//		{
//			return weight > other.weight;
//		};
//	};
//	
//
//	int width, height;
//
//	// 2D �̷� ������
//	vector<vector<int>> mazeMap;
//
//	// �ʱ�ȭ
//	void InitializeMaze();
//
//	// ��ǥ ��ȿ�� �˻�
//	bool isValid(int x, int y)
//	{
//		return x > 0 && x < width - 1 && y > 0 && y < height - 1;
//	}
//};