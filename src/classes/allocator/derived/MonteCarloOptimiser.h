#ifndef MONTECARLO_H
#define MONTECARLO_H

using namespace std;

#include "../base/Allocator.h"

class MonteCarloOptimiser : public Allocator
{
public:
    MonteCarloOptimiser()
    {
        srand(time(NULL));
    }

    void optimise(int numSimulations);
    void simulateCharge(Vehicle *vehicle);
};

void MonteCarloOptimiser::optimise(int numSimulations)
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
            simulateCharge(&vehicle);

            // Check second recharge
            simulateCharge(&vehicle);
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
         << endl;

    Header::displayChargingStationQueueHeader();
    display.printAvgWaitingTime(getOverallWaitingTime());
}

void MonteCarloOptimiser::simulateCharge(Vehicle *vehicle)
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