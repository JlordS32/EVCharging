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
    
public:

    Vehicle(int vehicleId, int destinationId, int capacityRange, int remainRange)
    {
        this->vehicleId = vehicleId;
        this->currentCityId = 0;
        this->destinationId = destinationId;
        this->capacityRange = capacityRange;
        this->remainRange = remainRange;
    }

    void print();
    bool needToCharge();
    int calculateTravelDistance();
};

void Vehicle::print()
{
    cout << setw(5) << vehicleId;
    cout << setw(20) << nameMap[currentCityId];
    cout << setw(20) << nameMap[destinationId];
    cout << setw(20) << capacityRange;
    cout << setw(20) << remainRange;
    cout << endl;
}

bool Vehicle::needToCharge() {
    if (calculateTravelDistance() >= remainRange) {
        return true;
    }

    return false;
}

int Vehicle::calculateTravelDistance() {
    int totalDistance;

    for (int i = this->currentCityId; i < this->destinationId; i++) {
        totalDistance += distanceMap[i];
    }

    return totalDistance;
}

#endif