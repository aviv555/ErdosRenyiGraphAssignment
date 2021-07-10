#include <iostream>
#include <queue>
#include <time.h>
#include "Graph.h"

Graph::Graph(int vertexAmount)
{
	this->vertexAmount = vertexAmount;

	std::vector<int> vec;
	vec.reserve(vertexAmount / 2);

	this->neighboursArray.resize(vertexAmount, vec);
}


void Graph::randomize(float probability)
{
	for (int i = 0; i < this->vertexAmount; i++)
	{
		for (int j = i + 1; j < this->vertexAmount; j++)
		{
			float random = (static_cast<float>(rand())) / RAND_MAX; //random number between 0 to 1

			if (random <= probability)
			{
				this->neighboursArray[i].push_back(j);
				this->neighboursArray[j].push_back(i);
			}
		}
	}
}

int Graph::diameter() const
{
	int max = 0;
	for (int i = 0; i < this->vertexAmount; i++)
	{
		std::vector<int> dist = this->BFS(i);
		for (int distance : dist)
		{
			if (distance == -1)
				return INT_MAX;

			if (distance > max)
				max = distance;
		}
	}
	return max;
}

bool Graph::connectivity() const
{
	return !(this->isIsolated());
}

bool Graph::isIsolated() const
{
	for (int i = 0; i < this->vertexAmount; i++)
	{
		if (this->neighboursArray[i].empty())
			return true;
	}
	return false;
}


void Graph::print() const
{
	for (int i = 0; i < this->vertexAmount; i++)
	{
		std::cout << "VERTEX - " << i << "[";
		const auto& neighbours = this->neighboursArray[i];

		if (neighbours.empty())
		{
			std::cout << "]" << std::endl;
			continue;
		}

		int j = 0;
		for (; j < neighbours.size() - 1; j++)
			std::cout << neighbours[j] << ",";

		std::cout << neighbours[j] << "]" << std::endl;
	}
}

std::vector<int> Graph::BFS(const int& root) const
{
	std::queue<int> queue;
	std::vector<int> dist(this->vertexAmount, -1);
	std::vector<bool> visited(this->vertexAmount, false);

	dist[root] = 0;
	visited[root] = true;
	queue.push(root);

	while (!queue.empty())
	{
		const int currentVertex = queue.front();
		queue.pop();
		const auto& neighbours = this->neighboursArray[currentVertex];
		int nextDistance = dist[currentVertex] + 1;

		for (int neighbour : neighbours)
		{
			if (!visited[neighbour])
			{
				visited[neighbour] = true;
				dist[neighbour] = nextDistance;
				queue.push(neighbour);
			}
		}
	}
	return dist;
}
