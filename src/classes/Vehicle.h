#ifndef VEHICLE_H
#define VEHICLE_H

#include "../assets/Constant.h"
#include <iomanip>
#include <vector>

using namespace std;

#include "ChargingStation.h"

class Vehicle
{
private:
    int vehicleId;
    int currentCityId;
    int destinationId;
    int capacityRange;
    int remainRange;
    vector<ChargingStation *> stations;

public:
    Vehicle(int vehicleId, int currentCityId, int destinationId, int capacityRange, int remainRange)
    {
        this->vehicleId = vehicleId;
        this->currentCityId = currentCityId;
        this->destinationId = destinationId;
        this->capacityRange = capacityRange;
        this->remainRange = remainRange;
    }

    void print();
    int farthestDistance();

    // Getters
    int getVehicleId();
    int getCurrentCityId();
    int getDestinationId();
    int getCapacity();
    int getRemainingRange();
    ChargingStation* getCharger(int id);

    // Setters
    void fillUp(int capacity);
    void updateLocation(int currentLocationId);
    void resetState(Vehicle vehicle);
    void charge(ChargingStation *station);
};

void Vehicle::print()
{
    cout << setw(5) << vehicleId;
    cout << setw(20) << nameMap[currentCityId];
    cout << setw(20) << nameMap[destinationId];
    cout << setw(20) << capacityRange;
    cout << setw(20) << remainRange;
}

int Vehicle::farthestDistance()
{
    int totalDistance = 0;
    int range = this->remainRange;

    for (int i = this->currentCityId; i <= this->destinationId; i++)
    {
        if (range < 0)
        {
            return totalDistance;
        }

        totalDistance += distanceMap[i];
        range -= distanceMap[i];
    }

    return totalDistance;
}

int Vehicle::getVehicleId()
{
    return this->vehicleId;
}

int Vehicle::getCurrentCityId()
{
    return this->currentCityId;
}
int Vehicle::getDestinationId()
{
    return this->destinationId;
}

int Vehicle::getCapacity()
{
    return this->capacityRange;
}

int Vehicle::getRemainingRange()
{
    return this->remainRange;
}

void Vehicle::fillUp(int capacity)
{
    if (capacity <= 0)
    {
        cout << "\n\nError: Vehicle " << this->vehicleId << " cannot fill up. Capacity must be greater than 0." << endl;
        exit(1);
    }

    if (capacity > this->capacityRange)
    {
        cout << "\n\nError: Vehicle " << this->vehicleId << " cannot fill up. Capacity range exceeded." << endl;
        exit(1);
    }

    this->remainRange = capacity;
}

void Vehicle::updateLocation(int currentLocationId)
{
    this->currentCityId = currentLocationId;
}

void Vehicle::resetState(Vehicle vehicle)
{
    this->currentCityId = vehicle.currentCityId;
    this->capacityRange = vehicle.capacityRange;
    this->remainRange = vehicle.remainRange;
}

void Vehicle::charge(ChargingStation *station) {
    this->stations.push_back(station);
}

ChargingStation* Vehicle::getCharger(int id) {
    return this->stations[id];
}

#endif