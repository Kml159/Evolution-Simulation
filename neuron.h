#include <iostream>
#include "static.h"

using namespace std;

// Forward declaration
#define maxConnection 26
#define maxInnerNeuron 6
#define genomeLength 100

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

    // ADD LOCATION NEURON holds the coordiantes !!!
    LATERAL_LOCATION,
    VERTICAL_LOCATION,

    // MUSCLE
    GO_LEFT,
    GO_RIGHT,
    GO_UP,
    GO_DOWN,

    // LAST TYPE!
    KILL // This should be last neuron always

};

const static unsigned int NumberOfNeuronTypes = (int)(NeuronTypes::KILL)+1;

/*

    To Do's:

    - When go right and go left neurons have higher than 0.0 output, bigger one will be chosen !!!!!!!
    - Muslce neurons only fires in first step, FIX THIS !!!!!!!!!!!!!!!!!!
*/

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
    
    void conditionallyDo() {
        // If output is positive do the action
        if(output > 0){
            unconditionallyDo();
        }
    }    
    
    virtual void unconditionallyDo() = 0;
    virtual ~neuron() {}

    inline void accumulateInput(const double input){accumulation += input;}
    
    inline void reset(){
        // if(this == nullptr){throw invalid_argument("Neuron does not exist!");}
        accumulation = 0.0; 
        output = 0.0;
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
        else if(row > creatureTable->size()-1 || row < 0 || col > creatureTable->at(0).size()-1 || col < 0){
            // cout << "col: " << col << "\tcreatureTable->at(0).size()-1: " << creatureTable->at(0).size()-1 << endl;

            return true;
        }
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
        if(isOutOfBounds(coord->first, coord->second-1)){return -1.0;}                               // If there is a wall
        if(isOccupied(coord->first, coord->second-1)){return 1.0;}      // If there is a creature
        return 0.0;
    }

    void unconditionallyDo() override {}
};

struct rightEye: neuron{
    // Override output function

    double getOutput() const override {
        // If there is a creature right of the current creature return 1.0, else return 0.0
        if(isOutOfBounds(coord->first, coord->second+1)){return -1.0;}                               // If there is a wall
        if(isOccupied(coord->first, coord->second+1)){return 1.0;}      // If there is a creature
        return 0.0;
    }

    void unconditionallyDo() override {}
};

struct topEye: neuron{
    double getOutput() const override {
        // If there is a creature above the current creature return 1.0, else return 0.0
        if(isOutOfBounds(coord->first-1, coord->second)){return -1.0;}                               // If there is a wall
        if(isOccupied(coord->first-1, coord->second)){return 1.0;}      // If there is a creature
        return 0.0;
    }

    void unconditionallyDo() override {}
};

struct bottomEye: neuron{
    double getOutput() const override {
        // If there is a creature below the current creature return 1.0, else return 0.0
        if(isOutOfBounds(coord->first+1, coord->second)){return -1.0;}                               // If there is a wall
        if(isOccupied(coord->first+1, coord->second)){return 1.0;}      // If there is a creature
        return 0.0;
    }

    void unconditionallyDo() override {}
};

// SHOULD NEURONS NOT RETURN NEGATIVE VALUES ????? !!!!!
struct lateralLocation: neuron{

    double getOutput() const override {
        // Left: -1.0, Center: 0.0, Right: 1.0
        int location = coord->second;
        int size = creatureTable->at(0).size();
        return continuous(size, location);
    }

    void unconditionallyDo() override {}
};

struct verticalLocation: neuron{

    double getOutput() const override {
        // Top: -1.0, Center: 0.0, Bottom: 1.0
        int location = coord->first;
        int size = creatureTable->size();
        return continuous(size, location);
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
        coord->second--;                                                                            // Update the coordinates
    }
};

struct goRight: neuron{
    
    void unconditionallyDo() override {
        int row = coord->first;
        int col = coord->second + 1;
        if(isOutOfBounds(row, col) || isOccupied(row, col)){return;}
        swap(creatureTable->at(coord->first).at(coord->second), creatureTable->at(row).at(col)); // Swap to the right, so it moves.
        coord->second++;                                                                            // Update the coordinates
    }
};

struct goUp: neuron{
    
    void unconditionallyDo() override {
        int row = coord->first - 1;
        int col = coord->second;
        if(isOutOfBounds(row, col) || isOccupied(row, col)){return;}
        swap(creatureTable->at(coord->first).at(coord->second), creatureTable->at(row).at(col)); // Swap to the top, so it moves.
        coord->first--;                                                                            // Update the coordinates
    }
};

struct goDown: neuron{
    
    void unconditionallyDo() override {
        int row = coord->first + 1;
        int col = coord->second;
        if(isOutOfBounds(row, col) || isOccupied(row, col)){return;}
        swap(creatureTable->at(coord->first).at(coord->second), creatureTable->at(row).at(col)); // Swap to the bottom, so it moves.
        coord->first++;                                                                            // Update the coordinates
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
