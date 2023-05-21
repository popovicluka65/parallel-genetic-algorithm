#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using namespace std;

class GeneticAlgorithm
{
	vector<string> classroom;
	int numberWorkingDays;
	vector<string> classes;
	vector<int> duration;
	int popularizationSize;
	int generation;
	double mutationRate;
	int elitism;
public:
	GeneticAlgorithm(vector<string>& classroom, int numberWorkingDays, vector<string>& classes, vector<int>& duration);
	void optimization(vector<vector<string>>& bestInduvidual, vector<vector<int>>& bestStartsClass);
	void createPopulation(std::vector<std::vector<std::vector<std::string>>>& population, std::vector<std::vector<std::vector<int>>>& startsClassAll);
	void rate1(std::vector<std::vector<std::vector<std::string>>>& population, std::vector<std::vector<std::vector<int>>>& startClassAll, vector<int>& rates);
	void rate2(std::vector<std::vector<std::vector<std::string>>>& population, std::vector<std::vector<std::vector<int>>>& startClassAll, vector<int>& rates);
	void rulleteSelection(std::vector<std::vector<std::vector<std::string>>>& population, std::vector<std::vector<std::vector<int>>>& startClassAll, vector<int>& rates, std::vector<std::vector<std::vector<std::string>>>& selection);
	void mutation(vector<vector<vector<string>>>& population);
	int minimumNumber(int a, int b, int c);
};