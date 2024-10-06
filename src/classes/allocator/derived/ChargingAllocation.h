#ifndef CHARGINGALLOCATION_H
#define CHARGINGALLOCATION_H

using namespace std;

#include "../base/Allocator.h"

class ChargingAllocation : public Allocator
{
public:
    void chargeVehicles();
    void simulateCharge(Vehicle *vehicle);
};

void ChargingAllocation::chargeVehicles()
{
    Header::displayVehicleHeader();
    this->display.printVehicles();

    Header::displayChargingStationHeader();
    this->display.printChargingStations();

    Header::displayChargingAllocationHeader();
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

    Header::displayChargingStationQueueHeader();
    this->display.printAvgWaitingTime(getOverallWaitingTime());
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


#endif