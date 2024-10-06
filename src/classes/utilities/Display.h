#ifndef DISPLAY_H
#define DISPLAY_H

/**
 * @file Display.h
 * @brief Defines the Display class for managing and displaying information about vehicles and charging stations.
 */

#include <iostream>
#include <vector>

using namespace std;

#include "../Vehicle.h"
#include "../ChargingStation.h"

/**
 * @class Display
 * @brief Handles the display of vehicles and charging stations information.
 */
class Display
{
private:
    vector<Vehicle> *vehicles;           ///< Pointer to a vector of Vehicle objects.
    vector<ChargingStation> *stations;   ///< Pointer to a vector of ChargingStation objects.

public:
    /**
     * @brief Constructs a Display object with given vehicles and charging stations.
     * @param vehicles Pointer to a vector of Vehicle objects.
     * @param stations Pointer to a vector of ChargingStation objects.
     */
    Display(vector<Vehicle> *vehicles, vector<ChargingStation> *stations);

    /**
     * @brief Prints information about all vehicles.
     */
    void printVehicles();

    /**
     * @brief Prints information about all charging stations.
     */
    void printChargingStations();

    /**
     * @brief Prints the average waiting time at each charging station and overall.
     * @param overallWaitingTime The overall average waiting time for all vehicles.
     */
    void printAvgWaitingTime(double overallWaitingTime);

    /**
     * @brief Prints the charge allocation for each vehicle.
     */
    void printChargeAllocation();
};

Display::Display(vector<Vehicle> *vehicles, vector<ChargingStation> *stations)
    : vehicles(vehicles), stations(stations) {}

void Display::printVehicles()
{
    // Iterate through each vehicle and call its print function.
    for (Vehicle &vehicle : *this->vehicles)
    {
        vehicle.print();
        cout << endl;
    }
}

void Display::printChargingStations()
{
    // Iterate through each charging station and call its print function.
    for (ChargingStation &station : *this->stations)
    {
        station.print();
        cout << endl;
    }
}

void Display::printAvgWaitingTime(double overallWaitingTime)
{
    // Iterate through each charging station to display its average waiting time.
    for (ChargingStation &station : *this->stations)
    {
        station.print();

        cout << setw(25) << station.getQueueLength();
        cout << setw(20) << station.getAvgWaitingTime() << " hours";
        cout << endl;
    }

    // Display the overall average waiting time.
    cout << "\nOVERALL AVG WAITING TIME FOR EACH VEHICLE: " << overallWaitingTime << " hrs";
}

void Display::printChargeAllocation()
{
    // Iterate through each vehicle to display its charge allocation.
    for (Vehicle &vehicle : *this->vehicles)
    {
        vehicle.print();

        // Get the first charging station allocated to the vehicle.
        ChargingStation *firstCharger = vehicle.getCharger(0);
        if (firstCharger != nullptr)
            cout << setw(20) << firstCharger->getCityName();
        else
            cout << setw(20) << "----";

        // Get the second charging station allocated to the vehicle.
        ChargingStation *secondCharger = vehicle.getCharger(1);
        if (secondCharger != nullptr)
            cout << setw(20) << secondCharger->getCityName();
        else
            cout << setw(20) << "----";

        cout << endl;
    }
}

#endif
