#include <vector>
#include <unordered_map>
#include "func.h"

using namespace std;

#define maxConnection 16
#define maxInnerNeuron 2
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

    // LAST TYPE!
    KILL // This should be last neuron always

};

const static unsigned int NumberOfNeuronTypes = (int)(NeuronTypes::KILL)+1;

// INNER NEURON

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

// MUSCLE NEURONS
template <typename T>
struct goLeft: neuron{
    double output(unsigned int row, unsigned int col, vector<vector<T*>> &mat){
        
    }
};

template <typename T>
struct goRight: neuron{
    double output(unsigned int row, unsigned int col, vector<vector<T*>> &mat){
        
    }
};

template <typename T>
struct goUp: neuron{
    double output(unsigned int row, unsigned int col, vector<vector<T*>> &mat){
        
    }
};

template <typename T>
struct goDown: neuron{
    double output(unsigned int row, unsigned int col, vector<vector<T*>> &mat){
        
    }
};

template <typename T>
struct kill: neuron{
    double output(unsigned int row, unsigned int col, vector<vector<T*>> &mat){
        
    }
};

// INNER NEURONS
struct inner: neuron{

};

static const int ID_SIZE = 7;           // Adjust size as needed
static const int WEIGHT_SIZE = 32;      // Adjust size as needed
static const int DNA_SIZE = (1+ID_SIZE)*2 + WEIGHT_SIZE;

static const double CROSS_OVER_RATIO = 0.2;
static const double MUTATION_RATIO = 0.1;

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

    void crossOver(genome& A){
        for(int i=0; i < CROSS_OVER_RATIO*DNA_SIZE; i++){
            int index = getRandom(0, DNA_SIZE);
            swap(DNA[index], A.DNA[index]);
        }
    }

    void mutation(){
        for(int i=0; i < CROSS_OVER_RATIO*DNA_SIZE; i++){
            int index = getRandom(0, DNA_SIZE);
            DNA[index] = !DNA[index];
        }
    }

};

struct NN{

    neuron* neurons[NumberOfNeuronTypes+maxInnerNeuron];    // All types of neurons and X number of inner neuron already initialized
    genome DNA[maxConnection];

    NN(){
        // Initialize Neurons
        for(int i=0; i < NumberOfNeuronTypes; i++){
            // create all neurons using createNonInnerNeuron()
        }
    }

    neuron* createNonInnerNeuron(NeuronTypes type) {
        switch (type) {
            case NeuronTypes::LEFT_EYE:
                return new leftEye<creature>();
            case NeuronTypes::RIGHT_EYE:
                return new rightEye<creature>();
            case NeuronTypes::TOP_EYE:
                return new topEye<creature>();
            case NeuronTypes::BOTTOM_EYE:
                return new bottomEye<creature>();
            case NeuronTypes::GO_LEFT:
                return new goLeft<creature>(); // Add more cases for other neuron types
            case NeuronTypes::GO_RIGHT:
                return new goRight<creature>();
            case NeuronTypes::GO_UP:
                return new goUp<creature>();
            case NeuronTypes::GO_DOWN:
                return new goDown<creature>();
            case NeuronTypes::KILL:
                return new kill<creature>();
            // Add more cases for other neuron types
            default:
                throw invalid_argument("Neuron type is unknown!");
        }
    }


    void initNeurons(){
        // Connect neurons based on DNA
        for(int i=0; i < maxConnection; i++){
            auto SOURCE = DNA[i].getSource();
            auto DESTINATION = DNA[i].getDestination();

            if(SOURCE.first == true){
                NeuronTypes type = static_cast<NeuronTypes>(SOURCE.second%NumberOfNeuronTypes);
                // Input Neuron
                switch (type){
                    case NeuronTypes::LEFT_EYE:
                        // code
                        break;
                    case NeuronTypes::RIGHT_EYE:
                        // code
                        break;
                    case NeuronTypes::TOP_EYE:
                        // code
                        break;
                    case NeuronTypes::BOTTOM_EYE:
                        // code
                        break;
                    case NeuronTypes::GO_LEFT:
                        // code
                        break;
                    case NeuronTypes::GO_RIGHT:
                        // code
                        break;
                    case NeuronTypes::GO_UP:
                        break;
                    case NeuronTypes::GO_DOWN:
                        break;
                    case NeuronTypes::KILL:
                        break;
                    default:
                        throw invalid_argument("Source neuron type is incorrect!");
                        break;
                    }
            }
            else{
                // Create INNER NEURON
            }
        }
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
