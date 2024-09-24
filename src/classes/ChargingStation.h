#ifndef CHARGINGSTATION_H
#define CHARGINGSTATION_H

#include <iostream>
#include <iomanip>
#include "../assets/Constant.h"
using namespace std;

class ChargingStation
{
private:
    int cityId;
    string cityName;
    int distanceToLastCity;
    int numberOfChargers;

public:
    ChargingStation(int station)
    {
        this->cityId = station;
        this->cityName = nameMap[station];
        this->distanceToLastCity = distanceMap[station];
        this->numberOfChargers = chargersMap[station];
    }

    int distanceToSydney(int cityId);
    void print();
};

int ChargingStation::distanceToSydney(int cityId)
{
    int totalDistance;

    for (int i = 0; i <= cityId; i++)
    {
        totalDistance += distanceMap[i];
    }

    return totalDistance;
}

void ChargingStation::print()
{
    cout << setw(5) << this->cityId << setw(25);
    cout << this->cityName << setw(15);
    cout << distanceToSydney(this->cityId) << setw(25);
    cout << this->numberOfChargers << endl;
}

#endif