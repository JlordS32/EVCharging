#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <iomanip>

using namespace std;

#include "Utility.h"

class Header
{
public:
    static void displayVehicleHeader()
    {
        cout << "\n\nVEHICLE INFORMATION: " << endl;
        cout << Utility::headerBuilder(91);
        cout << "Vehicle Id" << setw(15)
             << "Origin" << setw(20)
             << "Destination" << setw(25)
             << "Capacity Range" << setw(20)
             << "Remaining Range" << endl;
        cout << Utility::headerBuilder(91);
    }

    static void displayChargingStationHeader()
    {
        cout << "\n\nCHARGING STATION INFORMATION: " << endl;
        cout << Utility::headerBuilder(82);
        cout << "Location Id" << setw(20)
             << "Location Name" << setw(25)
             << "Distance to Last City" << setw(25)
             << "Number of Chargers" << endl;
        cout << Utility::headerBuilder(82);
    }

    static void displayChargingAllocationHeader()
    {
        cout << "\n\nInitialising charging allocation..." << endl;
        cout << Utility::headerBuilder(131);
        cout << "Vehicle Id" << setw(15)
             << "Origin" << setw(20)
             << "Destination" << setw(25)
             << "Capacity Range" << setw(20)
             << "Remaining Range" << setw(20)
             << "First Recharge" << setw(20)
             << "Second Recharge" << endl;
        cout << Utility::headerBuilder(131);
    }

    static void displayChargingStationQueueHeader()
    {
        cout << "\n\nAVERAGE WAITING TIME: " << endl;
        cout << Utility::headerBuilder(122);
        cout << "Location Id" << setw(20)
             << "Location Name" << setw(25)
             << "Distance to Last City" << setw(25)
             << "Number of Chargers" << setw(20)
             << "Queue Length" << setw(20)
             << "Waiting Hours" << endl;
        cout << Utility::headerBuilder(122);
    }
};

#endif