#ifndef CHARGINGSTATION_H
#define CHARGINGSTATION_H

#include <iostream>
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
    ChargingStation(int cityId, string cityName, int distanceToLastCity, int numberOfChargers)
        : cityId(cityId),
          cityName(cityName),
          distanceToLastCity(distanceToLastCity),
          numberOfChargers(numberOfChargers)
    {
    }

    int distanceToSydney(int cityId);
};

int ChargingStation::distanceToSydney(int cityId) {
    int totalDistance;

    for (int i = 0; i <= cityId; i++) {
        totalDistance += distanceMap[i];
    }

    return totalDistance;
}


#endif