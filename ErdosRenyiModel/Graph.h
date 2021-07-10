#pragma once
#include <vector>

class Graph
{
private:
	std::vector<std::vector<int>> neighboursArray;
	int vertexAmount;

public:
	Graph(int vertexAmount);
	void randomize(float probability);
	int diameter() const;
	bool connectivity() const;
	bool isIsolated() const;
	void print() const;

private:
	std::vector<int> BFS(const int&) const;
};
