#ifndef CHARGINGALLOCATION_H
#define CHARGINGALLOCATION_H

#include <vector>
#include "../base/Allocator.h"

/**
 * @class ChargingAllocation
 * @brief Implements a specific charging allocation strategy for vehicles.
 *
 * The ChargingAllocation class derives from the Allocator base class and provides
 * an implementation for charging vehicles based on their remaining range and destination.
 */
class ChargingAllocation : public Allocator
{
public:
    /**
     * @brief Executes the charging process for all vehicles.
     *
     * This method displays vehicle and charging station information,
     * simulates the charging process for each vehicle, and displays the results.
     */
    void chargeVehicles();

    /**
     * @brief Simulates charging for a single vehicle.
     * @param vehicle Pointer to the Vehicle object to be charged.
     *
     * Determines if the vehicle needs charging based on its remaining range and
     * destination distance, finds the closest reachable charging station, and updates
     * the vehicle's state accordingly.
     */
    void simulateCharge(Vehicle *vehicle) override;
};

void ChargingAllocation::chargeVehicles()
{
    // Display headers and initial information
    Header::displayVehicleHeader();
    this->display.printVehicles();
    Header::displayChargingStationHeader();
    this->display.printChargingStations();
    Header::displayChargingAllocationHeader();

    // Save the initial state of vehicles
    vector<Vehicle> initialVehicles = this->vehicles;

    // Simulate charging for each vehicle
    for (Vehicle &vehicle : this->vehicles)
    {
        // Simulate first charge
        simulateCharge(&vehicle);

        // Simulate second charge
        simulateCharge(&vehicle);
    }

    // Display the charging allocation results
    this->display.printChargeAllocation();

    // Restore vehicles to their initial state
    this->vehicles = initialVehicles;

    // Display average waiting time
    Header::displayChargingStationQueueHeader();
    this->display.printAvgWaitingTime(getOverallWaitingTime());
}

void ChargingAllocation::simulateCharge(Vehicle *vehicle)
{
    // Get vehicle's current remaining range
    int remainingRange = vehicle->getRemainingRange();

    // Calculate distances
    int destinationDistance = this->stations[vehicle->getDestinationId()].distanceToSydney();
    int currentLocationDistance = this->stations[vehicle->getCurrentCityId()].distanceToSydney();
    int requiredDistance = destinationDistance - currentLocationDistance;

    // Check if the vehicle needs charging
    if (remainingRange < requiredDistance)
    {
        // Find the closest reachable charging station
        int closestStation = findClosestReachableStation(currentLocationDistance + remainingRange);

        if (closestStation >= 0)
        {
            // Allocate the charger at the closest station
            ChargingStation *charger = allocateCharger(closestStation);
            charger->incrementQueueLength();

            // Update vehicle's state after charging
            vehicle->charge(charger);
            vehicle->fillUp(vehicle->getCapacity());
            vehicle->updateLocation(closestStation);
        }
        else
        {
            // Error handling if no station is reachable
            cout << "\nError: Vehicle " << vehicle->getVehicleId()
                 << " cannot reach any charging station!" << endl;
        }
    }
}

#endif // CHARGINGALLOCATION_H
