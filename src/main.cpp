#include <iostream>
#include <vector>

using namespace std;

#include "./classes/FixedDemandDriver.h"
#include "./classes/ChargingStation.h"

const int NUM_STATIONS = 12;

int main() {
    FixedDemandDriver driver;
    vector<ChargingStation> stations;
    
    string fileName = driver.queryFile();
    driver.run("./assets/files/" + fileName);
    
    cout << "\n\n\nVEHICLE INFORMATION: " << endl;
    cout << "-------------------------------------------------------------------------------------------" << endl;
    cout << "Vehicle Id" << setw(15) 
         << "Origin" << setw(20)
         << "Destination" << setw(25)
         << "Capacity Range" << setw(20) 
         << "Remaining Range" << endl;
    cout << "-------------------------------------------------------------------------------------------" << endl;
    driver.print();

    cout << "\n\n\nCHARGING STATION INFORMATION: " << endl;
    cout << "-------------------------------------------------------------------------------------------" << endl;
    cout << "Location Id" << setw(20)
         << "Location Name" << setw(25)
         << "Distance to Last City" << setw(25)
         << "Number of Chargers" << endl;
    cout << "-------------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < NUM_STATIONS; i++) {
        stations.push_back(ChargingStation(i));
        stations[i].print();
    }

    return 0;
}