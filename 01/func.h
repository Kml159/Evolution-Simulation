#include "class.h"

using namespace std;

long double f(long double x, long double y){
    return -9*pow(x, 4) - 8*pow(y, 3) + 12*pow(x, 2) + 81*y + 12;
    return -2*pow(x, 2) + y;
}

template <class T>
void rShuffle(vector<T> &vec){

    for(int i=0; i < vec.size(); i++){

        int a = round(getRand<int>(0, vec.size()-1));
        int b = round(getRand<int>(0, vec.size()-1));

        swap(vec[a], vec[b]);
    }
}

individual crossover(individual parentA, individual parentB){
    
    individual child;
    child.x = (parentA.x + parentB.x)/2;
    child.y = (parentA.y + parentB.y)/2;
    child.calculateFitness();
    
    return child;
}

individual mutate(individual child, long double percent, float mRatio){

    if(getRand(0.0, 1.0) < mRatio){ // mutate

        // Example: parameter = 0.15, mutation should be between -0.15 and +0.15
        long double parameterX = abs(child.x * percent/100); 
        long double parameterY = abs(child.y * percent/100); 

        child.x = child.x + getRand<long double>(-parameterX, parameterX);
        child.y = child.y + getRand<long double>(-parameterY, parameterY);

        return child;

    }else{return child;}    
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

vector<individual> tournament(vector<individual> &population){ // takes already shuffled population

    vector<individual> childs;

    srand(time(0));
    rShuffle(population);

    for(int i=0; i < population.size(); i = i + 2){

        if(population[i].fitness > population[i+1].fitness){
            childs.push_back(population[i]);
        }else{
            childs.push_back(population[i+1]);
        }
    }

    return childs;
}