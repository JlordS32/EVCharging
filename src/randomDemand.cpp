#include <iostream>
#include <vector>

using namespace std;

#include "./classes/DemandGenerator.h"
#include "./classes/ChargingAllocation.h"

const int NUM_STATIONS = 12;

int main()
{
    DemandGenerator generate;

    // Generate random demands file
    string fileName = Utility::queryFile();
    generate.generateDemands(fileName);

    return 0;
}
