#include <iostream>
#include "Graph.h"
#include <future>
#include <vector>
#include <chrono>
#include <algorithm>

static const int V = 100;
static const int graphAmount = 500;
static const float threshold1 = (float)log(V) / V; //0.00690776
static const float threshold2 = (float)sqrt(2 * threshold1); //0.117539
static const float threshold3 = threshold1; //0.00690776

//first half of the array is smaller from threshold and second half is bigger
static const float probabilities[3][10] =
{ { 0.0037993f, 0.00449f, 0.0051808f, 0.0058716f, 0.0065624f, 0.0072531f, 0.0079439f, 0.0086347f, 0.0093255f, 0.010016f }, //probabilities for connectivity test
 { 0.064647f, 0.076401f, 0.088155f, 0.099908f, 0.11166f, 0.12342f, 0.13517f, 0.14692f, 0.15868f, 0.17043f}, //probabilities for diameter test
 { 0.0037993f, 0.00449f, 0.0051808f, 0.0058716f, 0.0065624f, 0.0072531f, 0.0079439f, 0.0086347f, 0.0093255f, 0.020026f } }; //probabilities for isolation test

static std::atomic<int> count;

static void diameterSimulationfunction(const Graph& graph)
{
	int diameter = graph.diameter();

	if (diameter == 2)
		count++;
}

static void connectivitySimulationfunction(const Graph& graph)
{
	bool result = graph.connectivity();

	if (result)
		count++;
}

static void isolationSimulationfunction(const Graph& graph)
{
	bool result = graph.isIsolated();

	if (result)
		count++;
}


static const std::vector<void(*)(const Graph&)> functionVector = { &connectivitySimulationfunction , &diameterSimulationfunction , &isolationSimulationfunction };


void RunTest(int ticket, std::string testName)
{
	std::cout << testName << " TEST" << std::endl;
	std::cout << "==============" << std::endl;

	auto row = probabilities[ticket];

	for (int i = 0; i < 10; i++)
	{
		float currentProbability = row[i];
		std::cout << "probability: " << currentProbability << " " << std::flush;

		std::vector<std::future<void>> futures;

		for (int j = 0; j < graphAmount; j++)
		{
			Graph graph(V);
			graph.randomize(currentProbability);
			futures.push_back(std::async(std::launch::async, functionVector[ticket], graph));
		}

		for (const auto& future : futures)
			future.wait();

		float result = (float)count / graphAmount;
		std::cout << "result:      " << result << std::endl;
		count = 0;
	}
	std::cout << "==============" << std::endl;
}


int main()
{
	srand(0);
	int input;

	while (true)
	{
		std::cout << "FOR CONNECTIVITY TEST ==> 0" << std::endl;
		std::cout << "FOR DIAMETER TEST ==> 1" << std::endl;
		std::cout << "FOR ISOLATION TEST ==> 2" << std::endl;
		std::cout << "FOR EXIT ==> -1" << std::endl;

		std::cin >> input;

		switch (input)
		{

		case -1:
			exit(0);

		case 0:
		{
			RunTest(0, "CONNECTIVITY");
			break;
		}

		case 1:
		{
			RunTest(1, "DIAMETER");
			break;
		}

		case 2:
		{
			RunTest(2, "ISOLATION");
			break;
		}

		default:
		{
			std::cout << "INVALID INPUT!" << std::endl << std::endl;
			break;
		}
		}
	}
	return 0;
}