#ifndef VEHICLE_H
#define VEHICLE_H

#include "../assets/Constant.h"
#include <iomanip>

using namespace std;

class Vehicle
{
private:
    int vehicleId;
    int currentCityId;
    int destinationId;
    int capacityRange;
    int remainRange;
    
    int calculateTravelDistance();
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
    int getRemainingRange();
    int getDestinationId();
};

void Vehicle::print()
{
    cout << setw(5) << vehicleId;
    cout << setw(20) << nameMap[currentCityId];
    cout << setw(20) << nameMap[destinationId];
    cout << setw(20) << capacityRange;
    cout << setw(20) << remainRange;
}

int Vehicle::farthestDistance() {
    int totalDistance;
    int range = this->remainRange;

    for (int i = this->currentCityId; i < this->destinationId; i++) {
        totalDistance += distanceMap[i];
        range -= distanceMap[i];

        if (range <= 0) {
            return totalDistance;
        }
    }

    return totalDistance;
}

int Vehicle::getRemainingRange() {
    return this->remainRange;
}

int Vehicle::getDestinationId() {
    return this->destinationId;
}

#endif