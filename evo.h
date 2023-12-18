#include <vector>
#include <unordered_map>
#include "func.h"

using namespace std;

#define maxConnection 16
#define maxNeuron 4
#define genomeLength 100

enum class NeuronTypes{

    // INPUT 
    LEFT_EYE,
    RIGHT_EYE,
    TOP_EYE,
    BOTTOM_EYE,

    // MUSCLE
    GO_LEFT,
    GO_RIGHT,
    GO_UP,
    GO_DOWN,

    // INNER
    INNER,

    // OTHER ACTIONS
    KILL

};

/*
    100001
    ^input-muscle or inner neuron
     ^
*/

struct neuron{
    
    vector<pair<neuron*, double>> connections;
    double bias;
    double output;

    string neuronCode;

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

// INPUT NEURONS
struct leftEye: neuron{
    double output(unsigned int row, unsigned int col, vector<vector<creature*>> &mat) const {
        // Creature 
        if(row > mat.size()-1 || row < 0 || col > mat.at(0).size()-1 || col < 0){return -1.0;}   // If there is a wall
        if(mat.at(row).at(col-1) != nullptr){return 1.0;}                                        // If there is a creature
        else{return 0.0;}
    }
};

struct rightEye: neuron{
    double output(unsigned int row, unsigned int col, vector<vector<creature*>> &mat) const {
        // Creature 
        if(row > mat.size()-1 || row < 0 || col > mat.at(0).size()-1 || col < 0){return -1.0;}   // If there is a wall
        if(mat.at(row).at(col+1) != nullptr){return 1.0;}                                        // If there is a creature
        else{return 0.0;}
    }
};

struct topEye: neuron{
    double output(unsigned int row, unsigned int col, vector<vector<creature*>> &mat) const {
        // Creature 
        if(row > mat.size()-1 || row < 0 || col > mat.at(0).size()-1 || col < 0){return -1.0;}   // If there is a wall
        if(mat.at(row+1).at(col) != nullptr){return 1.0;}                                        // If there is a creature
        else{return 0.0;}
    }
};

struct bottomEye: neuron{
    double output(unsigned int row, unsigned int col, vector<vector<creature*>> &mat) const {
        // Creature 
        if(row > mat.size()-1 || row < 0 || col > mat.at(0).size()-1 || col < 0){return -1.0;}   // If there is a wall
        if(mat.at(row-1).at(col) != nullptr){return 1.0;}                                        // If there is a creature
        else{return 0.0;}
    }
};

// INNER NEURONS
struct inner: neuron{

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
