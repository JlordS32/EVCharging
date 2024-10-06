#ifndef DEMANDGENERATOR_H
#define DEMANDGENERATOR_H

/**
 * @file DemandGenerator.h
 * @brief Provides the DemandGenerator class for generating vehicle demands.
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

#include "../assets/Constant.h"

/**
 * @class DemandGenerator
 * @brief Generates vehicle demand data and writes it to a file.
 */
class DemandGenerator {
private:
    /**
     * @brief Generates a random integer within a specified range.
     * 
     * @param min The minimum value (inclusive).
     * @param max The maximum value (exclusive).
     * @param offset An optional offset to adjust the range (default is 0).
     * 
     * @return `int` - A random integer between min and max.
     */
    int generateRandom(int min, int max, int offset = 0);

    /**
     * @brief Prompts the user to enter the number of demands.
     * 
     * @return `int` - The validated number of demands entered by the user.
     */
    int queryDemand();
public:
    /**
     * @brief Generates vehicle demands and writes them to a specified file.
     * 
     * @param filename The name of the file to write the demands to.
     */
    void generateDemands(string filename);
};

void DemandGenerator::generateDemands(string filename) {
    // Seed the random number generator with the current time
    srand(time(NULL)); 
    
    ofstream fout;
    fout.open(filename);

    // Get the number of demands from the user
    int demand = queryDemand();  

    string separator = ",";
    for (int i = 0; i < demand; i++)
    {
        fout << "[";

        // Generate the values for each demand
        int vehicleID = 200 + i; 
        int destinationID = generateRandom(0, NUM_CITIES);
        int capacity = generateRandom(MIN_CAPACITY, MAX_CAPACITY, 1);
        int remainingRange = generateRandom(300, capacity, 1);

        // Write the demand data to the file
        fout << vehicleID << separator
             << destinationID << separator
             << capacity << separator 
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

    cout << "Enter the number of demands (" << MIN_DEMANDS << " - " << MAX_DEMANDS << "): ";
    cin >> numberOfDemands;

    // Validate the number of demands
    while (numberOfDemands < MIN_DEMANDS || numberOfDemands > MAX_DEMANDS)
    {
        cout << "Invalid number of demands. Please enter a number between " << MIN_DEMANDS << " and " << MAX_DEMANDS << ": ";
        cin >> numberOfDemands;
    }

    return numberOfDemands;
}

#endif
