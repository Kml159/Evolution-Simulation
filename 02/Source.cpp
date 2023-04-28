#include <iostream>
#include <cmath>
#include <cstdlib> // required for rand() and srand()
#include <ctime>   // required for time()
#include <vector>

#define maxB 10
#define minB -10
#define mutationProb 0.2
#define popNumber 10

using namespace std;

int getRandomNumber(int minBound, int maxBound) { // USE SEED in only main function!!!
    int range = maxBound - minBound + 1; // calculate the range
    int randomNumber = (rand() % range) + minBound; // generate the random number
    return randomNumber;
}

double f(double x, double y){ // the function
    return -2*pow(x, 2) + y;
}

class individual{
    public:
        double x;
        double y;
        double fitness;

        individual(){ 
            this->fitness = f(x, y);
            this->x = getRandomNumber(minB, maxB);
            this->y = getRandomNumber(minB, maxB);
        }     
}; 

void printPop(vector<individual> &pop);
void sort(vector<individual> &pop);
void mutate(individual ind);
void crossover(individual ind1, individual ind2);



int main(){

    srand(time(0)); // call only once

    vector<individual> pop;
    pop.resize(popNumber);

    return 0;
}

void printPop(vector<individual> pop){

    for(int i=0; i < pop.size(); i++){
        cout << i << ")\t";
        cout << "x: " << pop[i].x;
        cout << "\ty: " << pop[i].y << endl;
    }
}