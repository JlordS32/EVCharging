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
    ChargingStation* allocateCharger(int index);

public:
    void run(string fileName = "");
    void print();
    void needToRecharge();
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

void ChargingAllocation::print()
{
    for (Vehicle &vehicle : this->vehicles) {
        vehicle.print();
        cout << endl;
    }
}

void ChargingAllocation::needToRecharge()
{
    for (Vehicle &vehicle : this->vehicles)
    {
        vehicle.print();

        int farthestDistance = vehicle.farthestDistance();
        int destinationDistance = this->stations[vehicle.getDestinationId()].distanceToSydney();

        if (farthestDistance <= destinationDistance)
        {
            // Find the closest station
            int closestStation = findClosestReachableStation(vehicle.getRemainingRange());
            ChargingStation* charger = allocateCharger(closestStation);

            cout << setw(20) << charger->getCityName();
        } else {
            cout << setw(20) << "N/A";
        }

        cout << endl;
    }
}

int ChargingAllocation::findClosestReachableStation(int remainRange) {
    int closestStationIndex = -1;

    for (int i = 0; i < NUM_CITIES; i++) {
        if (stations[i].distanceToSydney() >= remainRange) {
            return i - 1;
        }
    }

    return closestStationIndex;
}

ChargingStation* ChargingAllocation::allocateCharger(int index) {
    return &this->stations[index];
}

#endif