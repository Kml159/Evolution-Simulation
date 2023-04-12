#include <iostream>
#include <cmath>
#include <cstdlib> // required for rand() and srand()
#include <ctime>   // required for time()
#include <vector>
#include <iomanip>
// #include <random>

using namespace std;

long double f(long double x, long double y){
    return 9*pow(x, 4) - 8*pow(y, 3) + 12*pow(x, 2) + 81*y + 12;
}

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
            fitness = 9*pow(this->x, 4) - 8*pow(this->y, 3) + 12*pow(this->x, 2) + 81*this->y + 12;
        }

        void initializeIndividual(long double minBound, long double maxBound){ 

            // x and y should take different bounds??

            srand(time(0));

            this->x = getRand(minBound, maxBound);
            this->y = getRand(minBound, maxBound);
        }        
}; 

class population{

    individual ind;
};

void initializePopulation(vector<long double> &pop, long double minBound, long double maxBound, unsigned popNumber){

    if(!pop.empty()){
        throw runtime_error("Error: Population vector is not empty");
    }

    for(int i=0; i < popNumber; i++){
        pop.push_back(getRand(minBound, maxBound));       
    }
}

template <class T>
T crossover(T parentA, T parentB){
    return (parentA + parentB)/2;
}

long double mutate(long double child, long double percent){

    // Example: parameter = 0.15, mutation should be between -0.15 and +0.15
    long double parameter = abs(child * percent/100); 

    return child + getRand<long double>(-parameter, parameter);
}

// selection

int main(){

    const unsigned int populationSize = 10;

    vector<individual> population(populationSize); 

    // Initialize Population
    for(int i=0; i < populationSize; i++){
        population[i].initializeIndividual(-100, 100);
        population[i].calculateFitness();
    }

    // 





    return 0;
}