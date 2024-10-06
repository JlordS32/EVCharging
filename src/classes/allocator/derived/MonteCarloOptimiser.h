#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <vector>
#include <ctime>
#include "../base/Allocator.h"

/**
 * @class MonteCarloOptimiser
 * @brief Implements a Monte Carlo optimization strategy for charging allocation.
 *
 * The MonteCarloOptimiser class uses random simulations to find an optimal charging
 * allocation that minimizes the overall average waiting time at charging stations.
 */
class MonteCarloOptimiser : public Allocator
{
public:
    /**
     * @brief Constructor for MonteCarloOptimiser class.
     *
     * Initializes the random number generator.
     */
    MonteCarloOptimiser()
    {
        srand(time(NULL));
    }

    /**
     * @brief Executes the Monte Carlo optimization process.
     * @param numSimulations Number of simulations to run.
     *
     * Performs multiple simulations to find the charging allocation that results
     * in the lowest average waiting time.
     */
    void optimise(int numSimulations);

    /**
     * @brief Simulates charging for a single vehicle with randomization.
     * @param vehicle Pointer to the Vehicle object to be charged.
     *
     * Determines if the vehicle needs charging, randomly selects a reachable
     * charging station, and updates the vehicle's state.
     */
    void simulateCharge(Vehicle *vehicle) override;
};

void MonteCarloOptimiser::optimise(int numSimulations)
{
    double improvedWaitingTime = numeric_limits<double>::max();
    int simulationCount = 0;

    // Initialize local copies of vehicles and stations
    vector<Vehicle> initialVehicles = this->vehicles;
    vector<Vehicle> tempVehicles = this->vehicles;
    vector<ChargingStation> improvedStations = this->stations;

    cout << "\n\nBalancing waiting queues with Monte-Carlo simulations..." << endl;

    // Run simulations
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
            // Simulate first charge
            simulateCharge(&vehicle);

            // Simulate second charge
            simulateCharge(&vehicle);
        }

        // Calculate the average waiting time
        double avgWaiting = getOverallWaitingTime();

        // Check if the current simulation has improved waiting time
        if (avgWaiting < improvedWaitingTime)
        {
            // Update best stations and vehicles
            improvedStations = this->stations;
            this->vehicles = tempVehicles;

            // Update improved waiting time
            improvedWaitingTime = avgWaiting;
            cout << "Improved Waiting Time: " << avgWaiting << " hours at simulation "
                 << simulationCount << endl;
        }

        simulationCount++;
    }

    // Display the optimized charging allocation
    Header::displayChargingAllocationHeader();
    display.printChargeAllocation();

    // Update stations with the best ones found
    this->stations = improvedStations;

    // Display the final average waiting time
    cout << "\nAverage Waiting Time after "
         << numSimulations << " simulations: "
         << improvedWaitingTime << " hours"
         << endl;

    // Display the charging station queue information
    Header::displayChargingStationQueueHeader();
    this->display.printAvgWaitingTime(getOverallWaitingTime());
}

void MonteCarloOptimiser::simulateCharge(Vehicle *vehicle)
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
        // Randomly determine total range for finding a station
        int totalRange = currentLocationDistance + rand() % remainingRange;

        // Find the closest reachable charging station
        int closestStation = findClosestReachableStation(totalRange);

        if (closestStation >= 0)
        {
            // Allocate the charger at the closest station
            ChargingStation *charger = allocateCharger(closestStation);
            charger->incrementQueueLength();

            // Calculate a random amount to fill up (between minimum and capacity)
            int minimum = static_cast<int>(vehicle->getCapacity() / 1.5) - 1;
            int fillAmount = minimum + rand() % (vehicle->getCapacity() - minimum);

            // Update vehicle's state after charging
            vehicle->charge(charger);
            vehicle->fillUp(fillAmount);
            vehicle->updateLocation(closestStation);
        }
        else
        {
            // Error handling if no station is reachable
            cout << "\nError: Vehicle " << vehicle->getVehicleId()
                 << " cannot reach any charging station!" << endl;
            cout << "Closest station: " << closestStation << endl;
            exit(1);
        }
    }
}

#endif // MONTECARLO_H
