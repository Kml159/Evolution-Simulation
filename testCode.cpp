#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <iomanip> 
#include <algorithm> 
#include "creature.h"

using namespace std;

int main(){

    srand(static_cast<unsigned>(time(nullptr)));

    creature A;

    A.printNeuronConnections();

    
    return 0;
}
