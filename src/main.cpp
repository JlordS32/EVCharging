#include <iostream>
#include <vector>

using namespace std;

#include "./classes/ChargingAllocation.h"
#include "./classes/ChargingStation.h"
#include "./classes/DemandGenerator.h"

const int NUM_STATIONS = 12;

void displayChargingStationHeader();

int main() {
    ChargingAllocation allocate;
    
    string fileName = allocate.queryFile();
    allocate.run(fileName);

    cout << "\n\nVEHICLE INFORMATION: " << endl;
    cout << Utility::headerBuilder(91);
    cout << "Vehicle Id" << setw(15) 
         << "Origin" << setw(20)
         << "Destination" << setw(25)
         << "Capacity Range" << setw(20) 
         << "Remaining Range" << endl;
    cout << Utility::headerBuilder(91);
    allocate.printVehicles();

    // displayChargingStationHeader();
    // for (int i = 0; i < NUM_STATIONS; i++) {
    //     stations.push_back(ChargingStation(i));
    //     stations[i].print();
    // }

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
    allocate.printChargeAllocation();

    return 0;
}

void displayChargingStationHeader() {
    cout << "\n\nCHARGING STATION INFORMATION: " << endl;
    cout << Utility::headerBuilder(131);
    cout << "Location Id" << setw(20)
         << "Location Name" << setw(25)
         << "Distance to Last City" << setw(25)
         << "Number of Chargers" << endl;
    cout << Utility::headerBuilder(131);
}