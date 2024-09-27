#include <iostream>
#include <vector>

using namespace std;

#include "./classes/ChargingAllocation.h"
#include "./classes/Utility.h"

int main()
{
     ChargingAllocation allocate;

     // string fileName = Utility::queryFile();
     allocate.run("RandomDemands.txt");

     // Print the results
     allocate.print();

     // Balance the charging allocation
     allocate.useMonteCarlo(5000);

     return 0;
}
