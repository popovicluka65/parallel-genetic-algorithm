#include <iostream>
#include "GeneticAlgorithm.h"
#include <vector>
#include <algorithm>
#include<random>
#include<string>
#include<tuple>
using namespace std;
GeneticAlgorithm::GeneticAlgorithm(vector<string>& classroom, int numberWorkingDays, vector<string>& classes, vector<int>& duration)
{
	this->classroom = classroom;
	this->numberWorkingDays = numberWorkingDays;
	this->classes = classes;
	this->duration = duration;
	this->popularizationSize = 1000;
	this->elitism = elitism;
	this->generation = 4;
	this->mutationRate = 0.2;
	this->elitism = 4;
}
void GeneticAlgorithm::optimization(vector<vector<string>>& bestInduvidual, vector<vector<int>>& bestStartsClass) {
	int bestRate = -1000;
	vector<vector<vector<string>>> population;
	vector<vector<vector<string>>> selection;
	vector<vector<vector<int>>> startsClassAll;
	vector<int> rates;
	createPopulation(population, startsClassAll);
	rate1(population, startsClassAll, rates);
	vector<vector<vector<string>>> elitsmPopularity;
	for (int i = 0; i < elitism; i++) {
		elitsmPopularity.push_back(population[i]);
	}
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

void GeneticAlgorithm::createPopulation(vector<vector<vector<string>>>& population,vector<vector<vector<int>>>& startsClassAll) {
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
			random_device rd;
			mt19937 generator(rd());
			uniform_int_distribution<int> distribution(0, 24);
			int randomBroj = distribution(generator);
			if (population[randomBroj].size() >= 0) {
				vector <string> vec;
				vector <int> vec1;
				vec.push_back(classes[i - 1]);
				vec1.push_back(duration[i - 1]);
				population[randomBroj].push_back(vec);
				startsClassAll[randomBroj].push_back(vec1);
			}
		}
	}
}
void GeneticAlgorithm::rate1(vector<vector<vector<string>>>& population, vector<vector<vector<int>>>& startClassAll, vector<int>& rates) {
	for (int i = 0; i < population.size() - 1; i++) {
		int sum = 0;
		vector<vector<string>> table = population[i];
		vector<vector<int>> time = startClassAll[i];
		for (int j = 0; j < time[i].size(); j++) {
			int multyply = 0;
			vector<string> a = table[j];
			if (a.size() == 0) {
				multyply = 720 * 720;
			}
			else {
				multyply = time[i][j] * (720 - time[i][j]);
			}
			sum = sum + multyply;
		}
		rates.push_back(sum);
	}
	for (int i = 0; i < rates.size() - 1; i++) {
		for (int j = i + 1; j < rates.size(); j++) {
			if (rates[j] > rates[i]) {
				population[i], population[j] = population[j], population[i];
				rates[i], rates[j] = rates[j], rates[i];
				startClassAll[i], startClassAll[j] = startClassAll[j], startClassAll[i];
			}
		}
	}
}

void GeneticAlgorithm::rate2(vector<vector<vector<string>>>& population,vector<vector<vector<int>>>& startClassAll, vector<int>& rates) {
	int min1 = minimumNumber(population.size(), rates.size(), startClassAll.size());
	for (int i = 0; i < min1 - 1; i++) {
		int sum = 0;
		vector<vector<string>> table = population[i];
		vector<vector<int>> time = startClassAll[i];
		for (int j = 0; j < time[i].size(); j++) {
			int multyply = 0;
			vector<string> a = table[j];
			if (a.size() == 0) {
				multyply = 720 * 720;
			}
			else {
				multyply = time[i][j] * (720 - time[i][j]);
			}
			sum = sum + multyply;
		}
		rates.push_back(sum);
	}
	int min = minimumNumber(population.size(), rates.size(), startClassAll.size());
	for (int i = 0; i < min - 1; i++) {
		for (int j = i + 1; j < min; j++) {
			if (rates[j] > rates[i]) {
				population[i], population[j] = population[j], population[i];
				rates[i], rates[j] = rates[j], rates[i];
				startClassAll[i], startClassAll[j] = startClassAll[j], startClassAll[i];
			}
		}
	}
}

int GeneticAlgorithm::minimumNumber(int broj1, int broj2, int broj3) {
	int minBroj = broj1;
	if (broj2 < minBroj) {
		minBroj = broj2;
	}
	if (broj3 < minBroj) {
		minBroj = broj3;
	}
	return minBroj;
}


void GeneticAlgorithm::mutation(vector<vector<vector<string>>>& population) {
	for (int i = 0; i < population.size(); i++) {
		vector<vector<string>> gen = population[i];
		if (gen.size() != 0) {
			for (int j = 0; j < gen.size(); j++) {
				random_device rd;
				mt19937 gen(rd());
				uniform_real_distribution<double> dis(0.0, 1.0);
				double randomNum = dis(gen);
				if (randomNum < mutationRate) {
					uniform_int_distribution<int> dis(0, population.size() - 1);
					int randomNum1 = dis(gen);
					int randomNum2 = dis(gen);
					population[i][randomNum1], population[i][randomNum2] = population[i][randomNum2], population[i][randomNum1];
				}
			}
		}
	}
}

void GeneticAlgorithm::rulleteSelection(vector<vector<vector<string>>>& population, vector<vector<vector<int>>>& startClassAll, vector<int>& rates,vector<vector<vector<string>>>& selection) {
	for (int i = 0; i < population.size(); i = i + 2) {
		vector<int> adapty;
		for (int j = 0; j < rates.size(); j++) {
			random_device rd;
			mt19937 generator(rd());
			double minBroj = 0.0;
			double maxBroj = 1.0;
			uniform_real_distribution<double> distribution(minBroj, maxBroj);
			double slucajanBroj = distribution(generator);
			int broj = slucajanBroj * rates[j];
			adapty.push_back(broj);
		}
		selection.push_back(population[0]);
		selection.push_back(population[1]);
	}
}