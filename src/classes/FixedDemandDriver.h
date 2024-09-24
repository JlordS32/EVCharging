#ifndef FIXEDDEMANDDRIVER_H
#define FIXEDDEMANDDRIVER_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include "Utility.h"
#include "Vehicle.h"
#include "../assets/Constant.h"

using namespace std;

class FixedDemandDriver {
private:
    vector<Vehicle> vehicles;

public:
    void run(string fileName = "");
    void print();
    string queryFile();
};

string FixedDemandDriver::queryFile() {
    string fileName;
    cout << "Enter file name: ";
    getline(cin, fileName);

    return fileName;
}

void FixedDemandDriver::run(string fileName) {
    ifstream fin;
    fin.open(fileName);

    if (!fin.is_open())
    {
        cout << "File not found!";
        exit(1);
    }

    string line;
    while (!fin.eof())
    {
        fin >> line;

        // Cleanup "[]"
        Utility::removeStr(line, "[");
        Utility::removeStr(line, "]");

        // split strings and store in array
        int size;
        string *parts = Utility::split(line, ",", size);

        // Dynamically assign each value into another variable
        int vehicleId = stoi(parts[0]);
        int destinationId = stoi(parts[1]);
        int capacityRange = stoi(parts[2]);
        int remainRange = stoi(parts[3]);

        // Create object
        Vehicle vehicle = Vehicle(vehicleId, destinationId, capacityRange, remainRange);
        this->vehicles.push_back(vehicle);
    }

    fin.close();
}

void FixedDemandDriver::print() {
    cout << "VEHICLE INFORMATION" << endl;
    cout << "-------------------------------------------------------------------------------------------" << endl;
    cout << "Vehicle Id" << setw(15) 
         << "Origin" << setw(20)
         << "Destination" << setw(25)
         << "Capacity Range" << setw(20) 
         << "Remaining Range" << endl;
    cout << "-------------------------------------------------------------------------------------------" << endl;
    
    for (int i = 0; i < this->vehicles.size(); i++) {
        this->vehicles[i].print();
    }
}

#endif