#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <vector>

using namespace std;

#include "Vehicle.h"
#include "ChargingStation.h"

class Display
{
private:
    vector<Vehicle> *vehicles;
    vector<ChargingStation> *stations;

public:
    Display(vector<Vehicle> *vehicles, vector<ChargingStation> *stations) : vehicles(vehicles), stations(stations) {}
    
    void printVehicles();
    void printChargingStations();
    void printAvgWaitingTime(double overallWaitingTime);
    void printChargeAllocation();
};

void Display::printVehicles()
{
    for (Vehicle &vehicle : *this->vehicles)
    {
        vehicle.print();
        cout << endl;
    }
}

void Display::printChargingStations()
{
    for (ChargingStation &station : *this->stations) {
        station.print();
        cout << endl;
    }
}

void Display::printAvgWaitingTime(double overallWaitingTime)
{
    for (ChargingStation &station : *this->stations)
    {
        station.print();

        cout << setw(25) << station.getQueueLength();
        cout << setw(20) << station.getAvgWaitingTime() << " hours";
        cout << endl;
    }

    cout << "\nOVERALL AVG WAITING TIME FOR EACH VEHICLE: " << overallWaitingTime << " hrs";
}

void Display::printChargeAllocation()
{
    for (Vehicle &vehicle : *this->vehicles)
    {
        vehicle.print();

        ChargingStation *firstCharger = vehicle.getCharger(0);
        if (firstCharger != nullptr)
            cout << setw(20) << firstCharger->getCityName();
        else
            cout << setw(20) << "----";

        ChargingStation *secondCharger = vehicle.getCharger(1);
        if (secondCharger != nullptr)
            cout << setw(20) << secondCharger->getCityName();
        else
            cout << setw(20) << "----";

        cout << endl;
    }
}

#endif