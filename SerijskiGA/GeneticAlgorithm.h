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
	GeneticAlgorithm(vector<string>& classroom, int numberWorkingDays, vector<string>& classes, vector<int>& duration, int popularizationSize, int generation, double mutationRate, int elitsm);
	void optimization(vector<vector<string>>& bestInduvidual, vector<vector<int>>& bestStartsClass);
	void createPopulation(std::vector<std::vector<std::vector<std::string>>>& population, vector<vector<vector<int>>>& startsClassAll);
	void rate1(std::vector<std::vector<std::vector<string>>>& population, vector<vector<vector<int>>>& startClassAll, vector<int>& rates);
	void rate2(std::vector<std::vector<std::vector<string>>>& population, vector<vector<vector<int>>>& startClassAll, vector<int>& rates);
	void rulleteSelection(std::vector<std::vector<std::vector<std::string>>>& population, vector<vector<vector<int>>>& startClassAll, vector<int>& rates, vector<vector<vector<string>>>& selection);
	void mutation(vector<vector<vector<string>>>& population);
};