#include <vector>
#include <unordered_map>
#include "func.h"

using namespace std;

#define maxConnection 16
#define maxNeuron 4
#define genomeLength 100

struct neuron{
    
    vector<pair<neuron*, double>> connections;
    double bias;
    double output;

    neuron(){
        bias = getRandomDouble(-1.0, 1.0);
    }

    void link(neuron* linked){
        if(connections.size() < maxConnection){
            connections.push_back(make_pair(linked, getRandomDouble(-3.0, 3.0)));
        }
        else {
            throw invalid_argument("Too much neuron");
        }
    }
};

struct NN{

    vector<neuron*> neurons;

    NN(){

    }

    ~NN(){
        for(int i=0; i < neurons.size(); delete neurons[i++]);
    }
};

struct creature{

    pair<int, int> coord;
    bool genome[genomeLength];
    NN brain;
    char symbol;
    string color;

    creature(){
        symbol = 'o'; // getRandom(1, 16) + '0';
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distribution(0, colorMap.size() - 1);
        color = next(begin(colorMap), distribution(gen))->second;
    }

    void randomize(){
        symbol = (getRandom(0, 5)) + 'a';
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distribution(0, colorMap.size() - 1);
        color = next(begin(colorMap), distribution(gen))->second;
    }

    Action randomAction() {
        return static_cast<Action>(getRandom(0, 6));
    }

};
