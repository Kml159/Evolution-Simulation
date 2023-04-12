#include "class.h"

using namespace std;

long double f(long double x, long double y){
    return 9*pow(x, 4) - 8*pow(y, 3) + 12*pow(x, 2) + 81*y + 12;
}

long double crossover(long double parentA, long double parentB){
    return (parentA + parentB)/2;
}

long double mutate(long double child, long double percent){

    // Example: parameter = 0.15, mutation should be between -0.15 and +0.15
    long double parameter = abs(child * percent/100); 

    return child + getRand<long double>(-parameter, parameter);
}

void sortPopulation(vector<individual> &population){

    // Bubble sort
    for(int i=0; i < population.size()-1; i++){
        for(int j=0; j < population.size()-i-1; j++){
            if(population[j].fitness > population[j+1].fitness){
                swap(population[j], population[j+1]);                
            }
        }
    }
}

void tournament(vector<individual> &population){ // takes already shuffled population

    for(int i=0; i < population.size(); i = i + 2){

        if(population[i].fitness > population[i+1].fitness){
            population.push_back(population[i]);
        }else{
            population.push_back(population[i+1]);
        }
    }
}