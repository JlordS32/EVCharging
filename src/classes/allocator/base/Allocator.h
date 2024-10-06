#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

#include "../../utilities/Utility.h"
#include "../../Vehicle.h"
#include "../../ChargingStation.h"
#include "../../utilities/Header.h"
#include "../../utilities/Display.h"
#include "../../../assets/Constant.h"

/**
 * @class Allocator
 * @brief Base class for allocation strategies of vehicles to charging stations.
 *
 * The Allocator class provides common functionalities for different allocation strategies.
 * It contains methods and attributes shared among derived classes.
 */
class Allocator {
protected:
    // Class attributes
    vector<Vehicle> vehicles;
    vector<ChargingStation> stations;
    Display display;

    // Class methods
    /**
     * @brief Finds the closest reachable charging station based on the remaining range.
     * 
     * @param remainRange The remaining range of the vehicle.
     * @return Index of the closest reachable charging station, or -1 if none is reachable.
     */
    int findClosestReachableStation(int remainRange);

    /**
     * @brief Allocates a charger at the specified index.
     * 
     * @param index Index of the charging station.
     * @return Pointer to the allocated ChargingStation object, or nullptr if index is invalid.
     */
    ChargingStation *allocateCharger(int index);

    /**
     * @brief Calculates the overall average waiting time across all charging stations.
     * @return The overall average waiting time.
     */
    double getOverallWaitingTime();

public:
    /**
     * @brief Reads vehicle data from a file and initializes vehicles.
     * 
     * @param fileName Name of the input file containing vehicle data.
     */
    void run(string fileName);

    /**
     * @brief Pure virtual function to simulate charging for a vehicle.
     * 
     * @param vehicle Pointer to the Vehicle object.
     */
    virtual void simulateCharge(Vehicle *vehicle) = 0;

    /**
     * @brief Constructor for Allocator class.
     *
     * Initializes the charging stations and the display object.
     */
    Allocator();

    /**
     * @brief Virtual destructor for Allocator class.
     */
    virtual ~Allocator() {};
};

Allocator::Allocator() : display(&this->vehicles, &this->stations)
{
    // Initialize charging stations
    for (int i = 0; i < NUM_CITIES; i++)
    {
        stations.push_back(ChargingStation(i));
    }
}

void Allocator::run(string fileName)
{
    ifstream fin;

    // Open file
    fin.open(fileName);

    // Validate file
    if (!fin.is_open())
    {
        cout << "File not found!";
        exit(1);
    }

    string line;
    while (!fin.eof())
    {
        fin >> line;

        // Cleanup "[]"
        Utility::removeStr(line, "[");
        Utility::removeStr(line, "]");

        // Split strings and store in array
        int size;
        string *parts = Utility::split(line, ",", size);

        // Extract vehicle information from parts
        int vehicleId = stoi(parts[0]);
        int destinationId = stoi(parts[1]);
        int capacityRange = stoi(parts[2]);
        int remainRange = stoi(parts[3]);

        // Create Vehicle object and add it to the list
        Vehicle vehicle = Vehicle(vehicleId, 0, destinationId, capacityRange, remainRange);
        this->vehicles.push_back(vehicle);

        // Clean up dynamically allocated memory
        delete[] parts;
    }

    // Close file
    fin.close();
}

int Allocator::findClosestReachableStation(int remainRange)
{
    int closestStationIndex = -1;

    // Iterate through the stations to find the closest reachable one
    for (int i = 1; i < NUM_CITIES; i++)
    {
        // If the station's distance to Sydney is greater or equal to remaining range,
        // the previous station is the closest reachable.
        if (this->stations[i].distanceToSydney() >= remainRange)
        {
            return i - 1;
        }
    }

    // Return -1 if no reachable station is found
    return closestStationIndex;
}

ChargingStation *Allocator::allocateCharger(int index)
{
    if (index >= 0 && index < stations.size())
    {
        // Return pointer to the charging station at the given index
        return &this->stations[index];
    }
    else
    {
        // Return nullptr if index is out of bounds
        return nullptr;
    }
}

double Allocator::getOverallWaitingTime()
{
    double n = 0, m = 0;

    // Calculate total queue length (n) and weighted sum of waiting times (m)
    for (ChargingStation &station : this->stations)
    {
        n += station.getQueueLength();
        m += station.getQueueLength() * station.getAvgWaitingTime();
    }

    // Check to prevent division by zero
    if (n == 0)
    {
        return 0;
    }

    // Calculate overall average waiting time and round to three decimal places
    return round((m / n) * 1000) / 1000;
}

#endif // ALLOCATOR_H
