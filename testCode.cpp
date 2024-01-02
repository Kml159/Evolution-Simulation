#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <iomanip> 
#include <algorithm> 
#include "creature.h"

using namespace std;

int main(){

    bool test[] = {1, 1, 1, 1, 1, 0, 0, 0};

    cout << boolArrayToUnsigned(test, 8) << endl;
    return 0;
}
