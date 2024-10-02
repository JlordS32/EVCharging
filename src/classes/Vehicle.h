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
        : vehicleId(vehicleId),
          currentCityId(currentCityId),
          destinationId(destinationId),
          capacityRange(capacityRange),
          remainRange(remainRange)
    {
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
    void resetState();
    void charge(ChargingStation *station);
};

/**
 * @brief Prints the vehicle information
 */
void Vehicle::print()
{
    cout << setw(5) << vehicleId;
    cout << setw(20) << nameMap[currentCityId];
    cout << setw(20) << nameMap[destinationId];
    cout << setw(20) << capacityRange;
    cout << setw(20) << remainRange;
}

/**
 * @brief Returns the farthest distance of the vehicle
 */
int Vehicle::farthestDistance()
{
    // Set initial variables
    int totalDistance = 0;
    int range = this->remainRange;

    // Loop through from current location to destination
    // then calculate the total distance the vehicle can travel
    // based on it's remaining range from current location.
    for (int i = this->currentCityId; i <= this->destinationId; i++)
    {
        // Cut off the loop if ranges reaches 0
        if (range < 0)
        {
            return totalDistance;
        }

        // Update total distance and range
        totalDistance += distanceMap[i];
        range -= distanceMap[i];
    }

    // Return totalDistance if loop doesn't terminate early.
    return totalDistance;
}

/**
 * @brief Getter for vehicleId
 */
int Vehicle::getVehicleId()
{
    return this->vehicleId;
}

/**
 * @brief Getter for currentCityId
 */
int Vehicle::getCurrentCityId()
{
    return this->currentCityId;
}

/**
 * @brief Getter for destinationId
 */
int Vehicle::getDestinationId()
{
    return this->destinationId;
}

/**
 * @brief Getter for capacity
 */
int Vehicle::getCapacity()
{
    return this->capacityRange;
}

/**
 * @brief Getter for remainRange
 */
int Vehicle::getRemainingRange()
{
    return this->remainRange;
}

/**
 * @brief Setter for remainRange
 * 
 * Added some custom constraints into this setter, 
 * such as making sure the capacity is between 0 and capacityRange.
 * 
 * @param capacity New remaining range value
 */
void Vehicle::fillUp(int capacity)
{
    // Guard clauses
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

    // Finally update value
    this->remainRange = capacity;
}

/**
 * @brief Setter for currentCityId
 * 
 * @param currentLocationId New currentCityId
 */
void Vehicle::updateLocation(int currentLocationId)
{
    this->currentCityId = currentLocationId;
}

/**
 * @brief Setter for stations
 * 
 * Order matters, so it's important to add the station to the back of the vector.
 */
void Vehicle::charge(ChargingStation *station) {
    this->stations.push_back(station);
}

/**
 * @brief Getter for stations
 * 
 * Returns the station specified.
 */
ChargingStation* Vehicle::getCharger(int id) {
    if (id >= 0 && id < stations.size())
        return this->stations[id];
    else
        return nullptr;
}

#endif