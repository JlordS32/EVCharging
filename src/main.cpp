#include <iostream>

#include "./classes/ChargingStation.h"
#include "./classes/Vehicle.h"

int main() {

    ChargingStation s(0, "Sydney", 2, 0);

    cout << s.distanceToSydney(2) << endl;

    return 0;
}