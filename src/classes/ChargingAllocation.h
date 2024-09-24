#ifndef CHARGINGALLOCATION_H
#define CHARGINGALLOCATION_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

#include "Utility.h"
#include "Vehicle.h"
#include "ChargingStation.h"
#include "../assets/Constant.h"

using namespace std;

class ChargingAllocation
{
private:
    vector<Vehicle> vehicles;
    vector<ChargingStation> stations;

    int findClosestReachableStation(int remainRange);
    ChargingStation *allocateCharger(int index);
    void checkCharge(Vehicle *vehicle);

public:
    void run(string fileName = "");
    void printVehicles();
    void printChargeAllocation();
    string queryFile();

    ChargingAllocation()
    {
        for (int i = 0; i < NUM_CITIES; i++)
        {
            stations.push_back(ChargingStation(i));
        }
    }
};

string ChargingAllocation::queryFile()
{
    string fileName;
    cout << "Enter file name: ";
    getline(cin, fileName);

    return fileName;
}

void ChargingAllocation::run(string fileName)
{
    ifstream fin;
    fin.open(fileName);

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

void ChargingAllocation::printVehicles()
{
    for (Vehicle &vehicle : this->vehicles)
    {
        vehicle.print();
        cout << endl;
    }
}

void ChargingAllocation::printChargeAllocation()
{
    for (Vehicle &vehicle : this->vehicles)
    {
        vehicle.print();

        int remainingRange = vehicle.getRemainingRange();
        int destinationDistance = this->stations[vehicle.getDestinationId()].distanceToSydney();

        // Check for first charge
        checkCharge(&vehicle);

        // Check for second charge
        checkCharge(&vehicle);

        cout << endl;
    }
}

void ChargingAllocation::checkCharge(Vehicle *vehicle)
{
    int farthestDistance = vehicle->farthestDistance();
    int destinationDistance = this->stations[vehicle->getDestinationId()].distanceToSydney();
    int currentLocationDistance = this->stations[vehicle->getCurrentCityId()].distanceToSydney();

    if (farthestDistance < (destinationDistance - currentLocationDistance))
    {
        // Find the closest station
        int closestStation = findClosestReachableStation(vehicle->getRemainingRange());
        ChargingStation *charger = allocateCharger(closestStation);
        vehicle->fillUp(vehicle->getCapacity());
        vehicle->updateLocation(closestStation);

        cout << setw(20) << charger->getCityName();
    }
    else
    {
        cout << setw(20) << "---";
    }
}

int ChargingAllocation::findClosestReachableStation(int remainRange)
{
    int closestStationIndex = -1;

    for (int i = 0; i < NUM_CITIES; i++)
    {
        if (stations[i].distanceToSydney() >= remainRange)
        {
            return i - 1;
        }
    }

    return closestStationIndex;
}

ChargingStation *ChargingAllocation::allocateCharger(int index)
{
    return &this->stations[index];
}

#endif