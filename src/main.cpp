#include <iostream>

using namespace std;

#include "./classes/FixedDemandDriver.h"

int main() {
    FixedDemandDriver driver;

    string fileName = driver.queryFile();
    driver.run("./assets/files/" + fileName);
    driver.print();

    return 0;
}