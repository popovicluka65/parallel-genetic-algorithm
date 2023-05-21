#include<iostream>
#include <fstream>
#include<string>
#include<vector>
#include<list>
#include <fstream>
#include <sstream>
#include <tbb/tick_count.h>
using namespace std;
using namespace tbb;

#include "GeneticAlgorithm.h"

vector<string> workingDays = { "Monday","Tuesday","Wednesday","Thursday","Friday" };

vector<string> splitString(const string& str, const string& delimiter) {
    vector<string> result;
    istringstream iss(str);
    string token;
    while (getline(iss, token, delimiter[0])) {
        result.push_back(token);
    }
    return result;
}

void loadDataClassroom(vector<string>& classroom) {
    std::ifstream file("data_timetable.txt");
    if (file.is_open()) {
        std::vector<std::string> contents;
        std::string line;
        while (std::getline(file, line)) {
            contents.push_back(line);
        }
        file.close();

        std::vector<std::string> class1;
        std::vector<std::string> class2;

        class1 = splitString(contents[0], "rooms: ");
        class2 = splitString(class1[1], "\n");
        classroom = splitString(class2[0], ", ");
    }
    else {
        std::cout << "Nije moguæe otvoriti datoteku." << std::endl;
    }
}
void loadDataClassDuration(vector<string>& classes, vector<int>& duration) {
    std::ifstream file("data_timetable.txt");
    string subject;
    string time;
    if (file.is_open()) {
        std::string line;
        int lineCount = 0;
        while (std::getline(file, line)) {
            if (lineCount >= 2) {
                istringstream iss(line);
                getline(iss, subject, ',');
                iss >> time;
                classes.push_back(subject);
                duration.push_back(std::stoi(time));
            }
            lineCount++;
        }
        file.close();
    }
    else {
        std::cout << "Nije moguæe otvoriti datoteku." << std::endl;
    }
}

void printSchedule(vector<vector<string>>& bestInduvidual, vector<vector<int>>& bestStartsClassvector, vector<string> classroom, vector<string> classes) {
    for (int i = 0; i < classes.size(); i++) {
        for (int j = 0; j < bestInduvidual[i].size(); j++) {
            cout << bestInduvidual[i][j] << endl;
        }
    }
}

int main()
{
    tick_count startTime = tick_count::now();
    vector<string> classroom;
    vector<string> classes;
    vector<int> duration;
    vector<vector<string>> bestInduvidual;
    vector<vector<int>> bestStartsClass;
    loadDataClassroom(classroom);
    loadDataClassDuration(classes, duration);
    GeneticAlgorithm geneticAlgorithm(classroom, workingDays.size(), classes, duration);
    geneticAlgorithm.optimization(bestInduvidual, bestStartsClass);
    printSchedule(bestInduvidual, bestStartsClass, classroom, classes);
    tick_count endTime = tick_count::now();
    cout << "Serial time: \t\t\t" << (endTime - startTime).seconds() << " seconds\n";
    string aaa;
    cin >> aaa;
}