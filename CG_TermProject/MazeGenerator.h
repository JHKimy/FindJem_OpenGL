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
//	// 프림 알고리즘으로 미로 생성
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
//	// 2D 미로 데이터
//	vector<vector<int>> mazeMap;
//
//	// 초기화
//	void InitializeMaze();
//
//	// 좌표 유효성 검사
//	bool isValid(int x, int y)
//	{
//		return x > 0 && x < width - 1 && y > 0 && y < height - 1;
//	}
//};