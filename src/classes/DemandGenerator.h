#ifndef DEMANDGENERATOR_H
#define DEMANDGENERATOR_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "../assets/Constant.h"
using namespace std;

class DemandGenerator {
private:
    int generateRandom(int min, int max, int offset = 0);
    int queryDemand();
public:
    void generateDemands(string filename);
};

void DemandGenerator::generateDemands(string filename) {
    srand(time(NULL)); 
    
    ofstream fout;
    fout.open(filename);

    int demand = queryDemand();

    string seperator = ",";
    for (int i = 0; i < demand; i++)
    {
        fout << "[";

        // Generate the values
        int vehicleID = 200 + i;
        int destinationID = generateRandom(0, NUM_CITIES);
        int capacity = generateRandom(MIN_CAPACITY, MAX_CAPACITY, 1);
        int remainingRange = generateRandom(300, capacity, 1);

        // Write to file
        fout << vehicleID << seperator
             << destinationID << seperator
             << capacity << seperator 
             << remainingRange;
        fout << "]" << endl;
    }

    fout.close();
}

int DemandGenerator::generateRandom(int min, int max, int offset)
{
    return min + (rand() % (max - min + offset));
}

int DemandGenerator::queryDemand()
{
    int numberOfDemands;

    cout << "Enter the number of demands (150 - 200): ";
    cin >> numberOfDemands;

    while (numberOfDemands < MIN_DEMANDS || numberOfDemands > MAX_DEMANDS)
    {
        cout << "Invalid number of demands. Please enter a number between 150 and 200: ";
        cin >> numberOfDemands;
    }

    return numberOfDemands;
}

#endif