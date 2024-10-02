#ifndef CHARGINGSTATION_H
#define CHARGINGSTATION_H

/**
 * @file ChargingStation.h
 * @brief Defines the ChargingStation class for managing charging station data.
 */

#include <iostream>
#include <iomanip>
#include <ctime>
#include "../assets/Constant.h"
using namespace std;

/**
 * @class ChargingStation
 * @brief Represents a charging station with its associated data and operations.
 */
class ChargingStation
{
private:
    int cityId;               ///< The unique identifier for the city.
    string cityName;          ///< The name of the city where the charging station is located.
    int distanceToLastCity;   ///< The distance from the last city to this city.
    int numberOfChargers;     ///< The number of chargers available at the station.
    int queueLength;          ///< The current queue length at the station.

public:
    /**
     * @brief Constructs a ChargingStation object for a given city.
     * @param station The city ID where the charging station is located.
     */
    ChargingStation(int station);

    /**
     * @brief Calculates the total distance from Sydney to this charging station.
     * @return The distance in kilometers.
     */
    int distanceToSydney();

    /**
     * @brief Calculates the total distance from Sydney to a specified city.
     * @param cityId The city ID to calculate the distance to.
     * @return The distance in kilometers.
     */
    int distanceToSydney(int cityId);

    /**
     * @brief Prints the charging station's information in a formatted manner.
     */
    void print();

    // Getters

    /**
     * @brief Retrieves the city ID of the charging station.
     * @return The city ID.
     */
    int getId();

    /**
     * @brief Retrieves the current queue length at the charging station.
     * @return The queue length.
     */
    int getQueueLength();

    /**
     * @brief Retrieves the name of the city where the charging station is located.
     * @return The city name.
     */
    string getCityName();

    /**
     * @brief Calculates the average waiting time at the charging station.
     * @return The average waiting time in hours.
     */
    double getAvgWaitingTime();

    // Setters

    /**
     * @brief Increments the queue length by one.
     */
    void incrementQueueLength();

    /**
     * @brief Resets the queue length to zero.
     */
    void resetQueue();
};

/**
 * @brief Constructs a ChargingStation object for a given city.
 * @param station The city ID where the charging station is located.
 */
ChargingStation::ChargingStation(int station)
{
    this->cityId = station;
    this->cityName = nameMap[station];
    this->distanceToLastCity = distanceMap[station];
    this->numberOfChargers = chargersMap[station];
    this->queueLength = 0;
}

/**
 * @brief Calculates the total distance from Sydney to a specified city.
 * @param cityId The city ID to calculate the distance to.
 * @return The distance in kilometers.
 */
int ChargingStation::distanceToSydney(int cityId)
{
    int totalDistance = 0;

    // Sum the distances from Sydney to the specified city
    for (int i = 0; i <= cityId; i++)
    {
        totalDistance += distanceMap[i];
    }

    return totalDistance;
}

int ChargingStation::distanceToSydney()
{
    // Use the cityId of this charging station
    return distanceToSydney(this->cityId);
}
void ChargingStation::print()
{
    cout << setw(5) << this->cityId << setw(25);
    cout << this->cityName << setw(15);
    cout << distanceToSydney(this->cityId) << setw(25);
    cout << this->numberOfChargers;
}

int ChargingStation::getId() {
    return this->cityId;
}

int ChargingStation::getQueueLength() {
    return this->queueLength;
}

double ChargingStation::getAvgWaitingTime() {
    // Average waiting time formula: (0.5 * queueLength) / numberOfChargers
    // Rounded to three decimal places
    return round(((0.5 * this->queueLength) / this->numberOfChargers) * 1000) / 1000;
}

void ChargingStation::incrementQueueLength() {
    this->queueLength++;
}

void ChargingStation::resetQueue() {
    this->queueLength = 0;
}

string ChargingStation::getCityName() {
    return this->cityName;
}

#endif
