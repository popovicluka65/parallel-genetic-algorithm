#include <iostream>
#include "GeneticAlgorithm.h"
#include <vector>
#include <algorithm>
#include<random>
#include<string>
#include<tuple>
#include <climits>
#include <algorithm>

using namespace std;
GeneticAlgorithm::GeneticAlgorithm(vector<string>& classroom, int numberWorkingDays, vector<string>& classes, vector<int>& duration, int popularizationSize, int generation, double mutationRate, int elitsm)
{
	this->classroom = classroom;
	this->numberWorkingDays = numberWorkingDays;
	this->classes = classes;
	this->duration = duration;
	this->popularizationSize = popularizationSize;
	this->generation = generation;
	this->mutationRate = mutationRate;
	this->elitism = elitism;
}
void GeneticAlgorithm::optimization(vector<vector<string>>& bestInduvidual, vector<vector<int>>& bestStartsClass) {
	int bestRate = INT_MIN;
	std::vector<std::vector<std::vector<std::string>>> population;
	std::vector<std::vector<std::vector<std::string>>> selection;
	std::vector<std::vector<std::vector<int>>> startsClassAll;
	vector<int> rates;
	createPopulation(population, startsClassAll);
	rate1(population, startsClassAll, rates);
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

		for (int j = 0; j < elitism; j++) {
			elitsmPopularity.push_back(population[i]);
		}

		mutation(population);
		rate2(population, startsClassAll, rates);

		if (rates[0] > bestRate) {
			bestRate = rates[0];
			bestInduvidual = population[0];
			bestStartsClass = startsClassAll[0];
		}
	}
}

void GeneticAlgorithm::createPopulation(std::vector<std::vector<std::vector<std::string>>>& population, std::vector<std::vector<std::vector<int>>>& startsClassAll) {
	for (int i = 0; i < numberWorkingDays * classroom.size(); i++) {
		vector < vector<string>> vec;
		vector < vector<int>> vec1;
		population.push_back(vec);
		startsClassAll.push_back(vec1);
	}
	for (int k = 0; k < popularizationSize; k++) {
		int i = 0;
		while (i < classes.size()) {
			i++;
			std::random_device rd;
			std::mt19937 generator(rd());
			std::uniform_int_distribution<int> distribution(0, 24);
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
}
void GeneticAlgorithm::rate1(std::vector<std::vector<std::vector<std::string>>>& population, std::vector<std::vector<std::vector<int>>>& startClassAll, vector<int>& rates) {
	for (int i = 0; i < population.size() - 1; i++) {
		int sum = 0;
		vector<vector<string>> table = population[i];
		vector<vector<int>> time = startClassAll[i];

		for (int j = 0; j < time[i].size(); j++) {
			int multyply = 0;
			vector<string> a = table[j];
			if (a.size() == 0) 
				multyply = 720 * 720;
			else 
				multyply = time[i][j] * (720 - time[i][j]);
			sum = sum + multyply;
		}

		rates.push_back(sum);
	}
	for (int i = 0; i < rates.size() - 1; i++) {
		for (int j = i + 1; j < rates.size(); j++) {
			if (rates[j] > rates[i]) {
				swap(population[i], population[j]);
				swap(rates[i], rates[j]);
				swap(startClassAll[i], startClassAll[j]);
			}
		}
	}
}

void GeneticAlgorithm::rate2(std::vector<std::vector<std::vector<std::string>>>& population, std::vector<std::vector<std::vector<int>>>& startClassAll, vector<int>& rates) {
	int length= std::min(population.size(), min(rates.size(), startClassAll.size()));

	for (int i = 0; i < length - 1; i++) {
		int sum = 0;
		vector<vector<string>> table = population[i];
		vector<vector<int>> time = startClassAll[i];
		for (int j = 0; j < time[i].size(); j++) {
			int multyply = 0;
			vector<string> a = table[j];
			if (a.size() == 0) 
				multyply = 720 * 720;
			else 
				multyply = time[i][j] * (720 - time[i][j]);
			
			sum = sum + multyply;
		}
		rates.push_back(sum);
	}
	int min = std::min(population.size(), std::min(rates.size(), startClassAll.size()));
	
	for (int i = 0; i < min - 1; i++) {
		for (int j = i + 1; j < min; j++) {
			if (rates[j] > rates[i]) {
				swap(population[i], population[j]);
				swap(rates[i], rates[j]);
				swap(startClassAll[i], startClassAll[j]);
			}
		}
	}
}

void GeneticAlgorithm::mutation(vector<vector<vector<string>>>& population) {
	for (int i = 0; i < population.size(); i++) {
		vector<vector<string>> gen = population[i];
		if (gen.size() != 0) {
			for (int j = 0; j < gen.size(); j++) {
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_real_distribution<double> dis(0.0, 1.0);
				double randomNum = dis(gen);
				if (randomNum < mutationRate) {
					std::uniform_int_distribution<int> dis(0, population.size() - 1);
					int rand1 = dis(gen);
					int rand2 = dis(gen);
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
			
			std::uniform_real_distribution<double> distribution(min,max);
			adapty.push_back((int)(distribution(generator) * rates[j]));
		}

		selection.push_back(population[0]);
		selection.push_back(population[1]);
	}
}