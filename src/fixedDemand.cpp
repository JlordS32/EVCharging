#include <iostream>
#include <vector>

using namespace std;

#include "./classes/allocator/derived/ChargingAllocation.h"
#include "./classes/allocator/derived/MonteCarloOptimiser.h"
#include "./classes/utilities/Utility.h"

int main()
{
     // Create objects for fixed allocation "ChargingAllocatin"
     // and optimiser "MonteCarloOptimiser"
     ChargingAllocation allocate;
     MonteCarloOptimiser optimise;
     
     // Quey user for file name
     string fileName = Utility::queryFile();
     
     // Run file for fixed charger allocation
     allocate.run(fileName);

     // Use optimiser
     optimise.run(fileName);

     // Print the results
     allocate.chargeVehicles();
     optimise.optimise(10000);

     return 0;
}
