#include <iostream>
#include "GeneticAlgorithm.h"
#include <vector>
#include <algorithm>
#include<random>
#include<string>
#include<tuple>
#include <algorithm>
#include <tbb\blocked_range.h>
#include <tbb\parallel_reduce.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_invoke.h>
#include <climits>

using namespace std;
using namespace tbb;
GeneticAlgorithm::GeneticAlgorithm(vector<string>& classroom, int numberWorkingDays, vector<string>& classes, vector<int>& duration, int popularizationSize, int generation, double mutationRate, int elitsm, int CUTOFF)
{
	this->classroom = classroom;
	this->numberWorkingDays = numberWorkingDays;
	this->classes = classes;
	this->duration = duration;
	this->popularizationSize = popularizationSize;
	this->generation = generation;
	this->mutationRate = mutationRate;
	this->elitism = elitism;
	this->CUTOFF = CUTOFF;
}
void GeneticAlgorithm::optimization(vector<vector<string>>& bestInduvidual, vector<vector<int>>& bestStartsClass) {
	int bestRate = INT_MIN;
	std::vector<std::vector<std::vector<std::string>>> population;
	vector < vector<string>> vec;
	population.push_back(vec);
	std::vector<std::vector<std::vector<std::string>>> selection;
	std::vector<std::vector<std::vector<int>>> startsClassAll;
	std::pair<std::vector<std::vector<std::vector<std::string>>>, std::vector<std::vector<std::vector<int>>>> children;
	vector<int> rates;
	children = parallelCreatePopulation(population, startsClassAll, numberWorkingDays * classroom.size());
	population = children.first;
	startsClassAll = children.second;

	rate(population, startsClassAll, rates);
	vector<vector<vector<string>>> elitsmPopularity;
	for (int i = 0; i < elitism; i++) 
		elitsmPopularity.push_back(population[i]);
	
	if (bestRate > rates[0]) {
		bestRate = rates[0];
		bestInduvidual = population[0];
	}
	for (int i = 0; i < generation; i++) {
		rulleteSelection(population, startsClassAll, rates, selection);
		population = selection;
		elitsmPopularity.clear();
		for (int j = 0; j < elitism; j++) 
			elitsmPopularity.push_back(population[i]);

		rate(population, startsClassAll, rates);

		if (rates[0] > bestRate) {
			bestRate = rates[0];
			bestInduvidual = population[0];
			bestStartsClass = startsClassAll[0];
		}
	}
}

std::pair<std::vector<std::vector<std::vector<std::string>>>, std::vector<std::vector<std::vector<int>>>> GeneticAlgorithm::parallelCreatePopulation
(std::vector<std::vector<std::vector<std::string>>>population, std::vector<std::vector<std::vector<int>>>startsClassAll, int parentsSize) {
	task_group g;
	std::pair<std::vector<std::vector<std::vector<std::string>>>, std::vector<std::vector<std::vector<int>>>> children;
	std::pair<std::vector<std::vector<std::vector<std::string>>>, std::vector<std::vector<std::vector<int>>>> children1;
	std::pair<std::vector<std::vector<std::vector<std::string>>>, std::vector<std::vector<std::vector<int>>>> children2;
	std::pair<std::vector<std::vector<std::vector<std::string>>>, std::vector<std::vector<std::vector<int>>>> children3;
	std::pair<std::vector<std::vector<std::vector<std::string>>>, std::vector<std::vector<std::vector<int>>>> children4;

	if (parentsSize <= GeneticAlgorithm::CUTOFF)
		children = createPopulation(population, startsClassAll, parentsSize);
	else
	{
		int size = parentsSize / 4;
		g.run([&] {children1 = parallelCreatePopulation(population, startsClassAll, size); });
		g.run([&] {children2 = parallelCreatePopulation(population, startsClassAll, size); });
		g.run([&] {children3 = parallelCreatePopulation(population, startsClassAll, size); });
		g.run([&] {children4 = parallelCreatePopulation(population, startsClassAll, size); });
		g.wait();
	}

	children.first.insert(children.first.end(), children1.first.begin(), children1.first.end());
	children.second.insert(children.second.end(), children1.second.begin(), children1.second.end());
	children.first.insert(children.first.end(), children2.first.begin(), children2.first.end());
	children.second.insert(children.second.end(), children2.second.begin(), children2.second.end());
	children.first.insert(children.first.end(), children3.first.begin(), children3.first.end());
	children.second.insert(children.second.end(), children3.second.begin(), children3.second.end());
	children.first.insert(children.first.end(), children4.first.begin(), children4.first.end());
	children.second.insert(children.second.end(), children4.second.begin(), children4.second.end());

	return children;
}

std::pair<std::vector<std::vector<std::vector<std::string>>>, std::vector<std::vector<std::vector<int>>>> GeneticAlgorithm::createPopulation
(std::vector<std::vector<std::vector<std::string>>>population, std::vector<std::vector<std::vector<int>>>startsClassAll, int size) {
	for (int i = 0; i < size; i++) {
		vector < vector<string>> vec_classes;
		vector < vector<int>> vec_duration;
		population.push_back(vec_classes);
		startsClassAll.push_back(vec_duration);
	}

	int length = min(population.size(), startsClassAll.size());
	
	for (int k = 0; k < popularizationSize; k++) {
		int i = 0;
		while (i < classes.size()) {
			i++;
			std::random_device rd;
			std::mt19937 generator(rd());
			std::uniform_int_distribution<int> distribution(0, length - 1);
			int rand = distribution(generator);
			if (population[rand].size() >= 0) {
				vector <string> vec_classes;
				vector <int> vec_duration;
				vec_classes.push_back(classes[i - 1]);
				vec_duration.push_back(duration[i - 1]);
				population[rand].push_back(vec_classes);
				startsClassAll[rand].push_back(vec_duration);
			}
		}
	}
	return make_pair(population, startsClassAll);
}

void GeneticAlgorithm::rate(std::vector<std::vector<std::vector<std::string>>>& population, std::vector<std::vector<std::vector<int>>>& startClassAll, vector<int>& rates) {
	int length = min(population.size(), startClassAll.size());
	for (int i = 0; i < length - 1; i++) {
		int sum = 0;
		vector<vector<string>> table = population[i];
		vector<vector<int>> time = startClassAll[i];
		int minSize = min(table.size(), time.size());
		for (int j = 0; j < minSize - 1; j++) {
			int multyply;
			vector<string> a = table[j];
			if (a.size() >= 0) 
				multyply = 720 * 720;
			sum = sum + multyply;
		}

		rates.push_back(sum);
	}
	int size = min(min(population.size(), rates.size()), startClassAll.size());
	for (int i = 0; i < size - 1; i++) {
		for (int j = i + 1; j < size; j++) {
			if (rates[j] > rates[i]) {
				swap(population[i], population[j]);
				swap(rates[i], rates[j]);
				swap(startClassAll[i], startClassAll[j]);
			}
		}
	}
}

void GeneticAlgorithm::mutation(vector<vector<vector<string>>>& population) {
	for (int i = 0; i < population.size() - 1; i++) {
		vector<vector<string>> gen = population[i];
		if (gen.size() > 1) {
			for (int j = 0; j < gen.size() - 1; j++) {
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_real_distribution<double> dis(0.0, 1.0);
				double randomNum = dis(gen);
				if (randomNum < mutationRate) {
					std::random_device rd;
					std::mt19937 generator(rd());
					std::uniform_int_distribution<int> distribution(1, j);
					int rand1 = distribution(generator);
					int rand2 = distribution(generator);
					swap(population[i][rand1], population[i][rand2]);
				}
			}
		}
	}
}

void GeneticAlgorithm::rulleteSelection(std::vector<std::vector<std::vector<std::string>>>& population, std::vector<std::vector<std::vector<int>>>& startClassAll, vector<int>& rates, std::vector<std::vector<std::vector<std::string>>>& selection) {
	double min = 0.0;
	double max = 1.0;
	for (int i = 0; i < population.size(); i = i + 2) {
		vector<int> adapty;
		for (int j = 0; j < rates.size(); j++) {
			std::random_device rd;
			std::mt19937 generator(rd());

			std::uniform_real_distribution<double> distribution(min, max);
			adapty.push_back((int)(distribution(generator) * rates[j]));
		}

		selection.push_back(population[0]);
		selection.push_back(population[1]);
	}
}