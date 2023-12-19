#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <iomanip> 
#include <algorithm> 
#include "evo.h"

using namespace std;

int main(){

    srand(static_cast<unsigned>(time(nullptr)));

    cout << (int)(NeuronTypes::KILL) << endl;

    return 0;

    genome A;
    cout << "DNA Size: " << (1+ID_SIZE)*2 + WEIGHT_SIZE << endl;
    cout << "src: " << A.getSource().first << " " << A.getSource().second << endl;
    cout << "dest: " << A.getDestination().first << " " << A.getDestination().second << endl;
    cout << "wgt: " << A.getWeight() << endl;

    for(int i=0; i < DNA_SIZE; i++){
        cout << A.DNA[i];
    }
    
}
