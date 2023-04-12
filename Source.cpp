#include "func.h"

using namespace std;

int main(){

    const unsigned int populationSize = 10;
    const unsigned int iteration      = 100;
    const unsigned int k              = 2;

    vector<individual> population(populationSize); 

    // Initialize Population
    for(int i=0; i < populationSize; i++){
        population[i].initializeIndividual(-100, 100);
        population[i].calculateFitness();
    }

    // Selection for mating process
    for(int i=0; i < iteration; i++){
        
    }

    // Sort according to fitness
    sortPopulation(population);    

    // Mutate

    // Selection

    return 0;
}