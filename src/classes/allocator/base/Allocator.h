#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <iostream>
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

class Allocator {
protected:
    // Attributes
    vector<Vehicle> vehicles;
    vector<ChargingStation> stations;
    Display display;

    // Protected methods
    int findClosestReachableStation(int remainRange);
    ChargingStation *allocateCharger(int index);
    double getOverallWaitingTime();

public:
    void run(string fileName);
    virtual void simulateCharge(Vehicle *vehicle) = 0;

    Allocator() : display(&this->vehicles, &this->stations)
    {
        for (int i = 0; i < NUM_CITIES; i++)
        {
            stations.push_back(ChargingStation(i));
        }
    }

    virtual ~Allocator() {};
};

void Allocator::run(string fileName)
{
    ifstream fin;
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

        // split strings and store in array
        int size;
        string *parts = Utility::split(line, ",", size);

        // Dynamically assign each value into another variable
        int vehicleId = stoi(parts[0]);
        int destinationId = stoi(parts[1]);
        int capacityRange = stoi(parts[2]);
        int remainRange = stoi(parts[3]);

        // Create object
        Vehicle vehicle = Vehicle(vehicleId, 0, destinationId, capacityRange, remainRange);
        this->vehicles.push_back(vehicle);

        delete[] parts;
    }

    fin.close();
}

int Allocator::findClosestReachableStation(int remainRange)
{
    int closestStationIndex = -1;

    for (int i = 1; i < NUM_CITIES; i++)
    {
        if (stations[i].distanceToSydney() >= remainRange)
        {
            return i - 1;
        }
    }

    return closestStationIndex;
}

ChargingStation *Allocator::allocateCharger(int index)
{
    if (index >= 0 && index < stations.size())
    {
        return &this->stations[index];
    }
    else
    {
        return nullptr;
    }
}

double Allocator::getOverallWaitingTime()
{
    double n, m = 0;

    // Calculate n and m
    for (ChargingStation &station : this->stations)
    {
        n += station.getQueueLength();
        m += station.getQueueLength() * station.getAvgWaitingTime();
    }

    return round((static_cast<double>(1 / n) * m) * 1000) / 1000;
}

#endif