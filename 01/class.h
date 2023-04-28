#include <iostream>
#include <cmath>
#include <cstdlib> // required for rand() and srand()
#include <ctime>   // required for time()
#include <vector>
#include <iomanip>
#include <algorithm>
#include <random> 

using namespace std;

template <class T>
T getRand(T minBound, T maxBound){
    return (T)std::rand() / RAND_MAX * (maxBound - minBound) + minBound;
}

class individual{
    public:
        long double x;
        long double y;

        unsigned int rank;

        unsigned int indSize;

        long double fitness;

        void calculateFitness(){ 
            fitness = -9*pow(this->x, 4) - 8*pow(this->y, 3) + 12*pow(this->x, 2) + 81*this->y + 12;
            fitness = -2*pow(x, 2) + y;
        }

        void initializeIndividual(long double minBound, long double maxBound){ 

            // x and y should take different bounds??

            srand(time(0));

            this->x = getRand(minBound, maxBound);
            this->y = getRand(minBound, maxBound);
        }        
}; 