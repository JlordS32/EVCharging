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
#include <cmath>

#include "Utility.h"
#include "Vehicle.h"
#include "ChargingStation.h"
#include "Header.h"
#include "Display.h"
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
    Display display;

public:
    void run(string fileName = "");
    void print();
    void chargeVehicles();
    void useMonteCarlo(int numSimulations);
    void simulateRandomCharge(Vehicle *vehicle);

    ChargingAllocation() : display(&this->vehicles, &this->stations)
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

void ChargingAllocation::print()
{
    Header::displayVehicleHeader();
    display.printVehicles();

    Header::displayChargingStationHeader();
    display.printChargingStations();

    Header::displayChargingAllocationHeader();
    chargeVehicles();

    Header::displayChargingStationQueueHeader();
    display.printAvgWaitingTime(getOverallWaitingTime());
}

void ChargingAllocation::chargeVehicles()
{
    // Save initial vehicle state
    vector<Vehicle> initialVehicles = this->vehicles;

    for (Vehicle &vehicle : this->vehicles)
    {
        // Assign first charger
        simulateCharge(&vehicle);

        // Assign second charger
        simulateCharge(&vehicle);
    }

    display.printChargeAllocation();

    // Restore
    this->vehicles = initialVehicles;
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

    for (int i = 1; i < NUM_CITIES; i++)
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
    double n, m = 0;

    // Calculate n and m
    for (ChargingStation &station : this->stations)
    {
        n += station.getQueueLength();
        m += station.getQueueLength() * station.getAvgWaitingTime();
    }

    return static_cast<double>(1 / n) * m;
}

void ChargingAllocation::useMonteCarlo(int numSimulations)
{
    srand(time(NULL));

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

    Header::displayChargingAllocationHeader();
    display.printChargeAllocation();

    // Finally update the stations in this class with the best ones.
    this->stations = improvedStations;

    // Output the average waiting time over all simuclations
    cout << "\nAverage Waiting Time after "
         << numSimulations << " simulations: "
         << improvedWaitingTime << " hours"
         << endl
         << endl;

    Header::displayChargingStationQueueHeader();
    display.printAvgWaitingTime(getOverallWaitingTime());
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
        int totalRange = currentLocationDistance + rand() % remainingRange;
        int closestStation = findClosestReachableStation(totalRange);

        if (closestStation >= 0)
        {
            // Get station and increment queue
            ChargingStation *charger = allocateCharger(closestStation);
            charger->incrementQueueLength();

            // Update remaining range to full and update location.
            int minimum = (vehicle->getCapacity() / 1.5) - 1;
            vehicle->charge(charger);
            vehicle->fillUp(minimum + rand() % (vehicle->getCapacity() - minimum));
            vehicle->updateLocation(closestStation);
        }

        // Error handling
        else
        {
            cout << "\nError: Vehicle " << vehicle->getVehicleId() << " cannot reach any charging station!" << endl;
            cout << "Closest station: " << closestStation << endl;
            exit(1);
        }
    }
}

#endif