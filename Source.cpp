#include "func.h"

using namespace std;

int main(){

    const unsigned int populationSize = 100;
    const unsigned int iteration      = 100;
    const unsigned int k              = 2;
    float mRatio                      = 0.4;

    // system("clear");

    vector<individual> population(populationSize); 

    // Initialize Population
    for(int i=0; i < populationSize; i++){
        population[i].initializeIndividual(-100, 100);
        population[i].calculateFitness();
    }

    for(int i=0; i < 100; i++){

        random_shuffle(population.begin(), population.end());

        vector<individual> childs = tournament(population);  // !!!

        // Mate and Mutate
        for(int i=0; i < populationSize; i = i + 2){
            population.push_back(mutate(crossover(childs[i], childs[i+1]), 20, mRatio));
        }

        // cout << "population.size(): " << population.size() << endl;
        // cout << "childs.size(): " << childs.size() << endl;

        childs.clear();

        // Sort according to fitness
        sortPopulation(population);   

        // Survive
        for(int i=population.size()-1; i >= populationSize; i--){
            population.pop_back();
        }

        // Print best
        cout << "Best Fitness:\t" << setprecision(20) << population[0].fitness << "\tX: " << population[0].x << "\tY: " << population[0].y << endl;
    }   


    return 0;
}