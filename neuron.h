#include <iostream>
#include "static.h"

using namespace std;

// Forward declaration
#define maxConnection 16
#define maxInnerNeuron 2
#define genomeLength 100

// Declaretions for reproduce function
#define CROSS_OVER_RATIO 0.2
#define MUTATION_RATIO 0.1

// Declaretions 
struct creature;
struct NN;
struct neuron;

// Declaretions for matrix 
vector<vector<creature*>>* creatureTable = nullptr;

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

struct neuron{
    
    double bias;
    double accumulation;
    double output;
    char genome[genomeLength];
    creature* owner = nullptr;

    pair<int, int> *coord;              // Pointer to the coordinates of the creature

    neuron(){
        // Initialize bias randomly
        bias = getRandomDouble(-1.0, 1.0);
        accumulation = 0.0;
    }

    neuron(neuron &neuron){
        // Copy Constructor
        this->bias = neuron.bias;
        this->accumulation = neuron.accumulation;
        this->output = neuron.output;
        this->coord = neuron.coord;
    }

    void setPTR(pair<int, int> &coord){this->coord = &coord;}
    
    // Pure virtual functions, must be implemented in derived classes
    void conditionallyDo() {
        // If output is positive do the action
        if(output > 0){
            if(owner != nullptr){reportAction(true);}
            unconditionallyDo();
        }
        else{
            if(owner != nullptr){reportAction(false);}
        }
    }    
    
    virtual void unconditionallyDo() = 0;
    virtual ~neuron() {}

    inline void accumulateInput(const double input){accumulation += input;}
    
    inline void reset(){
        accumulation = 0.0; 
        output = 0.0;
    }

    inline void reportAction(bool isActivated) const {
        // isActivated ? cout << GREEN_TEXT << typeid(*this).name() << " activated!" << RESET_TEXT : cout << RED_TEXT << typeid(*this).name() << " not activated!" << RESET_TEXT;
        if(isActivated){cout << GREEN_TEXT << setw(20) << typeid(*this).name() << RESET_TEXT;}
        else{cout << RED_TEXT << setw(20) << typeid(*this).name() << RESET_TEXT;}
        cout << "\t\tOutput: " << output << endl;
    }

    inline void calculateOutput(){
        // Calculate output by applying activation function to accumulation
        // output = activationFunctions::relu(bias + (W_1 * A_1 + W_2 * A_2 + W_3 * A_3 + ... + W_n * A_n));
        accumulation += bias;
        output = activationFunctions::relu(accumulation);
    }

    bool isOutOfBounds() const {
        // Check if the current neuron's coordinates are out of bounds
        if(creatureTable == nullptr){throw invalid_argument("creatureTable is nullptr!");} 
        else if(coord->first > creatureTable->size()-1 || coord->first < 0 || coord->second > creatureTable->at(0).size()-1 || coord->second < 0){return true;}
        return false; 
    }

    bool isOutOfBounds(int row, int col) const {
        // Check if the given coordinates are out of bounds
        if(creatureTable == nullptr){throw invalid_argument("creatureTable is nullptr!");} 
        else if(row > creatureTable->size()-1 || row < 0 || col > creatureTable->at(0).size()-1 || col < 0){return true;}
        return false; 
    }

    bool isOccupied(int row, int col) const {
        // Check if the given coordinates are occupied
        if(creatureTable == nullptr){throw invalid_argument("creatureTable is nullptr!");} 
        else if(creatureTable->at(row).at(col) != nullptr){return true;}
        return false;
    }

    inline virtual double getOutput() const {return output;}

    inline void print() const {
        cout << CYAN_TEXT << "Neuron:\t\t\t" << this << RESET_TEXT << endl;
        cout << typeid(*this).name() << endl;
        cout << "Bias:\t\t\t" << bias << endl;
        cout << "Accumulation:\t\t" << accumulation << endl;
        cout << "Output:\t\t\t" << output << endl;
        cout << endl;
    }

};

// INPUT NEURONS 
struct leftEye: neuron{       
    double getOutput() const override {
        // If there is a creature left of the current creature return 1.0, else return 0.0
        if(isOutOfBounds()){return -1.0;}                               // If there is a wall
        if(isOccupied(coord->first, coord->second-1)){return 1.0;}      // If there is a creature
        return 0.0;
    }

    void unconditionallyDo() override {}
};

struct rightEye: neuron{
    // Override output function

    double getOutput() const override {
        // If there is a creature right of the current creature return 1.0, else return 0.0
        if(isOutOfBounds()){return -1.0;}                               // If there is a wall
        if(isOccupied(coord->first, coord->second+1)){return 1.0;}      // If there is a creature
        return 0.0;
    }

    void unconditionallyDo() override {}
};

struct topEye: neuron{
    double getOutput() const override {
        // If there is a creature above the current creature return 1.0, else return 0.0
        if(isOutOfBounds()){return -1.0;}                               // If there is a wall
        if(isOccupied(coord->first+1, coord->second)){return 1.0;}      // If there is a creature
        return 0.0;
    }

    void unconditionallyDo() override {}
};

struct bottomEye: neuron{
    double getOutput() const override {
        // If there is a creature below the current creature return 1.0, else return 0.0
        if(isOutOfBounds()){return -1.0;}                               // If there is a wall
        if(isOccupied(coord->first-1, coord->second)){return 1.0;}      // If there is a creature
        return 0.0;
    }

    void unconditionallyDo() override {}
};

// MUSCLE NEURONS
struct goLeft: neuron{

    void unconditionallyDo() override { 
        int row = coord->first;
        int col = coord->second - 1;
        if(isOutOfBounds(row, col) || isOccupied(row, col)){return;}                                // If there is a wall or a creature, do nothing
        swap(creatureTable->at(coord->first).at(coord->second), creatureTable->at(row).at(col));    // Swap to the left, so it moves.
    }
};

struct goRight: neuron{
    
    void unconditionallyDo() override {
        int row = coord->first;
        int col = coord->second + 1;
        if(isOutOfBounds(row, col) || isOccupied(row, col)){return;}
        swap(creatureTable->at(coord->first).at(coord->second), creatureTable->at(row).at(col)); // Swap to the right, so it moves.
    }
};

struct goUp: neuron{
    
    void unconditionallyDo() override {
        int row = coord->first - 1;
        int col = coord->second;
        if(isOutOfBounds(row, col) || isOccupied(row, col)){return;}
        swap(creatureTable->at(coord->first).at(coord->second), creatureTable->at(row).at(col)); // Swap to the top, so it moves.
    }
};

struct goDown: neuron{
    
    void unconditionallyDo() override {
        int row = coord->first + 1;
        int col = coord->second;
        if(isOutOfBounds(row, col) || isOccupied(row, col)){return;}
        swap(creatureTable->at(coord->first).at(coord->second), creatureTable->at(row).at(col)); // Swap to the bottom, so it moves.
    }
};

struct kill: neuron{
    // Kill the creature in moving direction
    // IMPLEMENT THIS LATER
    void unconditionallyDo() override {}
};

// INNER NEURONS
struct inner: neuron{
    
    void unconditionallyDo() override {}
};
