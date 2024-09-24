#include <iostream>
#include <vector>

using namespace std;

#include "./classes/ChargingAllocation.h"
#include "./classes/ChargingStation.h"
#include "./classes/DemandGenerator.h"

const int NUM_STATIONS = 12;

void displayVehicleHeader();
void displayChargingStationHeader();

int main() {
    ChargingAllocation allocate;
    vector<ChargingStation> stations;
    DemandGenerator generator;
    
    string fileName = allocate.queryFile();
    allocate.run(fileName);

    displayVehicleHeader();
    allocate.print();

    displayChargingStationHeader();
    for (int i = 0; i < NUM_STATIONS; i++) {
        stations.push_back(ChargingStation(i));
        stations[i].print();
    }

    return 0;
}

void displayVehicleHeader() {
    cout << "\n\nVEHICLE INFORMATION: " << endl;
    cout << "-------------------------------------------------------------------------------------------" << endl;
    cout << "Vehicle Id" << setw(15) 
         << "Origin" << setw(20)
         << "Destination" << setw(25)
         << "Capacity Range" << setw(20) 
         << "Remaining Range" << endl;
    cout << "-------------------------------------------------------------------------------------------" << endl;
}

void displayChargingStationHeader() {
    cout << "\n\nCHARGING STATION INFORMATION: " << endl;
    cout << "-------------------------------------------------------------------------------------------" << endl;
    cout << "Location Id" << setw(20)
         << "Location Name" << setw(25)
         << "Distance to Last City" << setw(25)
         << "Number of Chargers" << endl;
    cout << "-------------------------------------------------------------------------------------------" << endl;
}