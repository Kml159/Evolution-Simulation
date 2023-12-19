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

    // LAST TYPE!
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

    char genome[genomeLength];

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
template <typename T>
struct leftEye: neuron{
    double output(unsigned int row, unsigned int col, vector<vector<T*>> &mat) const {
        if(row > mat.size()-1 || row < 0 || col > mat.at(0).size()-1 || col < 0){return -1.0;}   // If there is a wall
        if(mat.at(row).at(col-1) != nullptr){return 1.0;}                                        // If there is a creature
        else{return 0.0;}
    }
};

template <typename T>
struct rightEye: neuron{
    double output(unsigned int row, unsigned int col, vector<vector<T*>> &mat) const {
        if(row > mat.size()-1 || row < 0 || col > mat.at(0).size()-1 || col < 0){return -1.0;}   // If there is a wall
        if(mat.at(row).at(col+1) != nullptr){return 1.0;}                                        // If there is a creature
        else{return 0.0;}
    }
};

template <typename T>
struct topEye: neuron{
    double output(unsigned int row, unsigned int col, vector<vector<T*>> &mat) const {
        if(row > mat.size()-1 || row < 0 || col > mat.at(0).size()-1 || col < 0){return -1.0;}   // If there is a wall
        if(mat.at(row+1).at(col) != nullptr){return 1.0;}                                        // If there is a creature
        else{return 0.0;}
    }
};

template <typename T>
struct bottomEye: neuron{
    double output(unsigned int row, unsigned int col, vector<vector<T*>> &mat) const {
        if(row > mat.size()-1 || row < 0 || col > mat.at(0).size()-1 || col < 0){return -1.0;}   // If there is a wall
        if(mat.at(row-1).at(col) != nullptr){return 1.0;}                                        // If there is a creature
        else{return 0.0;}
    }
};

// INNER NEURONS
struct inner: neuron{

};

static const int ID_SIZE = 7; // Adjust size as needed
static const int WEIGHT_SIZE = 32; // Adjust size as needed
static const int DNA_SIZE = (1+ID_SIZE)*2 + WEIGHT_SIZE;

struct genome{

    /*
        SourceID:       1~0000000 // First bit: isInner, Rest: Which neuron
        DestinationID:  1~0000000 // First bit: isInner, Rest: Which neuron

        Genome:
        0-0011111-0-0000110-01111011010101110100010111000110
    */

    bool DNA[DNA_SIZE];

    genome(){
        // Initialize DNA randomly
        for(int i=0; i < DNA_SIZE; DNA[i++] = rand()%2);
    }

    pair<bool, unsigned int> getSource(){
        return make_pair(DNA[0], boolArrayToUnsigned(&DNA[1], ID_SIZE));
    }

    pair<bool, unsigned int> getDestination(){
        return make_pair(DNA[1+ID_SIZE], boolArrayToUnsigned(&DNA[2+ID_SIZE], ID_SIZE));
    }

    unsigned int getWeight(){
        return boolArrayToUnsigned(&DNA[2+(ID_SIZE*2)], WEIGHT_SIZE);
    }

};

struct NN{

    vector<neuron*> neurons;
    char genome[genomeLength];

    NN(){

    }

    ~NN(){
        for(int i=0; i < neurons.size(); delete neurons[i++]);
    }
};

struct creature{

    pair<int, int> coord;
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
