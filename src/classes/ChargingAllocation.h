#ifndef CHARGINGALLOCATION_H
#define CHARGINGALLOCATION_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>
#include <ctime>
#include <cstdlib>

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
    void simulateCharge(Vehicle *vehicle);
    double getOverallWaitingTime();

public:
    void run(string fileName = "");
    void printVehicles();
    void printChargingStations();
    void printChargeAllocation();
    void printChargeAllocation(string firstRecharge, string secondRecharge);
    void printAvgWaitingTime();
    void simulateMultipleScenarios(int numSimulations);
    void simulateRandomCharge(Vehicle *vehicle);

    ChargingAllocation()
    {
        for (int i = 0; i < NUM_CITIES; i++)
        {
            stations.push_back(ChargingStation(i));
        }
    }
};

void ChargingAllocation::run(string fileName)
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

void ChargingAllocation::printVehicles()
{
    for (Vehicle &vehicle : this->vehicles)
    {
        vehicle.print();
        cout << endl;
    }
}

void ChargingAllocation::printChargingStations()
{
    for (int i = 0; i < this->stations.size(); i++)
    {
        stations[i].print();
        cout << endl;
    }
}

void ChargingAllocation::printAvgWaitingTime()
{
    for (int i = 0; i < this->stations.size(); i++)
    {
        stations[i].print();

        cout << setw(25) << stations[i].getQueueLength();
        cout << setw(20) << stations[i].getAvgWaitingTime() << " hours";
        cout << endl;
    }

    cout << "\nOVERALL AVG WAITING TIME FOR EACH VEHICLE: " << getOverallWaitingTime() << " hrs";
}

void ChargingAllocation::printChargeAllocation()
{
    for (Vehicle &vehicle : this->vehicles)
    {
        vehicle.print();

        // Assign first charger
        simulateCharge(&vehicle);

        ChargingStation* firstCharger = vehicle.getCharger(0);
        if (firstCharger != nullptr)
            cout << setw(20) << firstCharger->getCityName();
        else
            cout << setw(20) << "----";

        // Assign second charger
        simulateCharge(&vehicle);

        ChargingStation* secondCharger = vehicle.getCharger(1);
        if (secondCharger != nullptr)
            cout << setw(20) << secondCharger->getCityName();
        else
            cout << setw(20) << "----";

        cout << endl;
    }
}


void ChargingAllocation::simulateCharge(Vehicle *vehicle)
{
    int remainingRange = vehicle->getRemainingRange();
    int destinationDistance = this->stations[vehicle->getDestinationId()].distanceToSydney();
    int currentLocationDistance = this->stations[vehicle->getCurrentCityId()].distanceToSydney();
    int requiredDistance = destinationDistance - currentLocationDistance;

    if (remainingRange < requiredDistance)
    {
        // Find the closest reachable station
        int closestStation = findClosestReachableStation(currentLocationDistance + remainingRange);

        if (closestStation >= 0)
        {
            // Get station and increment queue
            ChargingStation *charger = allocateCharger(closestStation);
            charger->incrementQueueLength();

            // Update remaining range to full and update location.
            vehicle->charge(charger);
            vehicle->fillUp(vehicle->getCapacity());
            vehicle->updateLocation(closestStation);
        }

        // Error handling
        else
        {
            cout << "\nError: Vehicle " << vehicle->getVehicleId() << " cannot reach any charging station!" << endl;
        }
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
    if (index >= 0 && index < stations.size())
    {
        return &this->stations[index];
    }
    else
    {
        return nullptr;
    }
}


double ChargingAllocation::getOverallWaitingTime()
{
    double n = 0;
    double m = 0;
    for (ChargingStation &station : this->stations)
    {
        n += station.getQueueLength();
        m += station.getQueueLength() * station.getAvgWaitingTime();
    }

    return static_cast<double>(1 / n) * m;
}


void ChargingAllocation::simulateMultipleScenarios(int numSimulations)
{
    double improvedWaitingTime = numeric_limits<double>::max();
    int simulationCount = 0;

    // Initialise a local copy
    vector<Vehicle> initialVehicles = this->vehicles;
    vector<Vehicle> tempVehicles = this->vehicles;
    vector<ChargingStation> improvedStations = this->stations;

    cout << "\n\nBalancing waiting queues with Monte-Carlo simulations..." << endl;
    while (simulationCount < numSimulations)
    {
        // Reset queues at the start of each simulation
        for (ChargingStation &station : stations)
        {
            station.resetQueue();
        }

        // Reset vehicles at the start of each simulation
        tempVehicles = initialVehicles;

        // Simulate charging for each vehicle
        for (Vehicle &vehicle : tempVehicles)
        {
            // Check first recharge
            simulateRandomCharge(&vehicle);

            // Check second recharge
            simulateRandomCharge(&vehicle);
        }

        double avgWaiting = getOverallWaitingTime();
        if (avgWaiting < improvedWaitingTime)
        {
            // Update best stations
            improvedStations = this->stations;

            // Update best vehicles
            this->vehicles = tempVehicles;

            // Get improved time
            improvedWaitingTime = avgWaiting;
            cout << "Improved Waiting Time: " << avgWaiting << " hours at " << simulationCount << endl;
        }
        simulationCount++;
    }

    // Finally update the stations in this class with the best ones.
    this->stations = improvedStations;

    // Output the average waiting time over all simulations
    cout << "\nAverage Waiting Time after "
         << numSimulations << " simulations: "
         << improvedWaitingTime << " hours"
         << endl
         << endl;

    printAvgWaitingTime();
}

void ChargingAllocation::simulateRandomCharge(Vehicle *vehicle)
{
    int remainingRange = vehicle->getRemainingRange();
    int destinationDistance = this->stations[vehicle->getDestinationId()].distanceToSydney(vehicle->getDestinationId());
    int currentLocationDistance = this->stations[vehicle->getCurrentCityId()].distanceToSydney(vehicle->getCurrentCityId());
    int currentDistance = destinationDistance - currentLocationDistance;

    if (remainingRange < currentDistance)
    {
        // Find the closest station
        int closestStation = findClosestReachableStation(currentLocationDistance + rand() % remainingRange);

        // Get station and increment queue
        ChargingStation *charger = allocateCharger(closestStation);
        charger->incrementQueueLength();

        // Update remaining range to full and update location.
        int minimum = (vehicle->getCapacity() / 2) - 1;
        vehicle->fillUp(minimum + rand() % (vehicle->getCapacity() - minimum));
        vehicle->updateLocation(closestStation);
    }
}

#endif