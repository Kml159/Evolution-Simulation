#include <vector>
#include <unordered_map>
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

// INNER NEURON
struct activationFunctions {
    // Activation functions are used to determine the output of a neuron
    static double sigmoid(double x) {
        return 1.0 / (1.0 + exp(-x));
    }

    static double relu(double x) {
        return max(0.0, x);
    }
};

struct neuron{
    // Neurons are the building blocks of neural networks
    // const static vector<vector<T*>> *s;
    double bias;
    double accumulation;
    double output;
    char genome[genomeLength];

    pair<int, int> *coord;              // Pointer to the coordinates of the creature

    neuron(){
        // Initialize bias randomly
        bias = getRandomDouble(-1.0, 1.0);
        accumulation = 0.0;
    }

    void setPTR(pair<int, int> &coord){
        this->coord = &coord;
    }
    
    // Pure virtual functions, must be implemented in derived classes
    virtual void conditionallyDo() = 0;
    virtual void unconditionallyDo() = 0;

    void accumulateInput(const double input){ 
        // input = output of source neuron * weight
        accumulation += input;
    }

    void calculateOutput(){
        // Calculate output by applying activation function to accumulation
        // output = activationFunctions::relu(bias + (W_1 * A_1 + W_2 * A_2 + W_3 * A_3 + ... + W_n * A_n));
        accumulation += bias;
        output = activationFunctions::relu(accumulation);
    }

    bool isOutOfBounds() const {
        // Check if the current neuron's coordinates are out of bounds
        if(coord->first > creatureTable->size()-1 || coord->first < 0 || coord->second > creatureTable->at(0).size()-1 || coord->second < 0){return true;}
        return false; 
    }

    bool isOccupied(int row, int col) const {
        // Check if the given coordinates are occupied
        if(creatureTable->at(row).at(col) != nullptr){return true;}
        return false;
    }

    double getOutput() const {return output;}

};

// NON-INNER NEURONS

// INPUT NEURONS 
struct leftEye: neuron{       
    double output() const {
        // If there is a creature left of the current creature return 1.0, else return 0.0
        if(isOutOfBounds()){return -1.0;}                               // If there is a wall
        if(isOccupied(coord->first, coord->second-1)){return 1.0;}      // If there is a creature
        return 0.0;
    }

    void unconditionallyDo() override {}
    void conditionallyDo() override {}
};

struct rightEye: neuron{
    double output() const {
        // If there is a creature right of the current creature return 1.0, else return 0.0
        if(isOutOfBounds()){return -1.0;}                               // If there is a wall
        if(isOccupied(coord->first, coord->second+1)){return 1.0;}      // If there is a creature
        return 0.0;
    }

    void unconditionallyDo() override {}
    void conditionallyDo() override {}
};

struct topEye: neuron{
    double output() const {
        // If there is a creature above the current creature return 1.0, else return 0.0
        if(isOutOfBounds()){return -1.0;}                               // If there is a wall
        if(isOccupied(coord->first+1, coord->second)){return 1.0;}      // If there is a creature
        return 0.0;
    }

    void unconditionallyDo() override {}
    void conditionallyDo() override {}
};

struct bottomEye: neuron{
    double output() const {
        // If there is a creature below the current creature return 1.0, else return 0.0
        if(isOutOfBounds()){return -1.0;}                               // If there is a wall
        if(isOccupied(coord->first-1, coord->second)){return 1.0;}      // If there is a creature
        return 0.0;
    }

    void unconditionallyDo() override {}
    void conditionallyDo() override {}
};

// MUSCLE NEURONS 
struct goLeft: neuron{

    void unconditionallyDo() override { 
        if(isOutOfBounds() || isOccupied(coord->first, coord->second-1)){return;}
        swap(creatureTable->at(coord->first).at(coord->second), creatureTable->at(coord->first).at(coord->second-1)); // Swap to the left, so it moves.
    }

    void conditionallyDo() override {
        // If output is positive do the action
        if(output > 0){unconditionallyDo();}
    }
};

struct goRight: neuron{
    
    void unconditionallyDo() override {
        if(isOutOfBounds() || isOccupied(coord->first, coord->second+1)){return;}
        swap(creatureTable->at(coord->first).at(coord->second), creatureTable->at(coord->first).at(coord->second+1)); // Swap to the second, so it moves.
    }

    void conditionallyDo() override {
        // If output is positive do the action
        if(output > 0){unconditionallyDo();}
    }
};

struct goUp: neuron{
    
    void unconditionallyDo() override {
        if(isOutOfBounds() || isOccupied(coord->first+1, coord->second)){return;}
        swap(creatureTable->at(coord->first).at(coord->second), creatureTable->at(coord->first+1).at(coord->second)); // Swap to the top, so it moves.
    }

    void conditionallyDo() override {
        // If output is positive do the action
        if(output > 0){unconditionallyDo();}
    }
};

struct goDown: neuron{
    
    void unconditionallyDo() override {
        if(isOutOfBounds() || isOccupied(coord->first-1, coord->second)){return;}
        swap(creatureTable->at(coord->first).at(coord->second), creatureTable->at(coord->first-1).at(coord->second)); // Swap to the bottom, so it moves.
    }

    void conditionallyDo() override {
        // If output is positive do the action
        if(output > 0){unconditionallyDo();}
    }
};

struct kill: neuron{
    // Kill the creature in moving direction
    // IMPLEMENT THIS LATER
    void unconditionallyDo() override {}
    void conditionallyDo() override {}
};

// INNER NEURONS
struct inner: neuron{

    void unconditionallyDo() override {}
    void conditionallyDo() override {}
};

static const int ID_SIZE = 7;          
static const int WEIGHT_SIZE = 32;     
static const int DNA_SIZE = (1+ID_SIZE)*2 + WEIGHT_SIZE;

struct genome{

    /*
        SourceID:       1~0000000 // First bit: isInner, Rest: Which source neuron is this?
        DestinationID:  1~0000000 // First bit: isInner, Rest: Which destination neuron is this?

        Genome:
        0-0011111-0-0000110-01111011010101110100010111000110
        ^SourceID ^DestID   ^Weight

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
        // Weight is 32 bits, so we need to convert it to unsigned int
        return boolArrayToUnsigned(&DNA[2+(ID_SIZE*2)], WEIGHT_SIZE);
    }

    void crossOver(genome& A){
        // Swap random bits between two genomes
        for(int i=0; i < CROSS_OVER_RATIO*DNA_SIZE; i++){
            int index = getRandom(0, DNA_SIZE);
            swap(DNA[index], A.DNA[index]);
        }
    }

    void mutation(){
        // Flip random bits
        for(int i=0; i < CROSS_OVER_RATIO*DNA_SIZE; i++){
            int index = getRandom(0, DNA_SIZE); // Get random index
            DNA[index] = !DNA[index];
        }
    }

};

struct NN{

    neuron* nonInnerNeurons[NumberOfNeuronTypes];    // All types of neurons and X number of inner neuron already initialized
    neuron* innerNeurons[maxInnerNeuron];            // Inner neurons are created dynamically
    genome DNA[maxConnection];

    pair<int, int> *coord;      // Pointer to the coordinates of the creature
    /*
        nonInnerNeurons:    0~3: Input Neurons
                            4~7: Muscle Neurons
                            8: Kill Neuron

        innerNeurons:       0~X: Inner Neurons

    */

    NN(){
        // Initialize nonInnerNeurons
        for(int i=0; i < NumberOfNeuronTypes; i++){
            nonInnerNeurons[i] = createNonInnerNeuron(static_cast<NeuronTypes>(i));
        }
        // Initialize innerNeurons
        for(int i=0; i < maxInnerNeuron; i++){
            innerNeurons[i] = new inner();
        }

        // Set pointers of neurons
        for(int i=0; i < NumberOfNeuronTypes; i++){
            nonInnerNeurons[i]->setPTR(*coord);
        }

        for(int i=0; i < maxInnerNeuron; i++){
            innerNeurons[i]->setPTR(*coord);
        }
    }

    void setPTR(pair<int, int> &coord){
        this->coord = &coord;
    }

    void action(){
        // Decode genomes and send inputs to neurons
        decodeGenomesAndSendInputs();
        // Do actions based on output of neurons
        for(int i=0; i < NumberOfNeuronTypes; i++){
            nonInnerNeurons[i]->conditionallyDo();
        }
        for(int i=0; i < maxInnerNeuron; i++){
            innerNeurons[i]->conditionallyDo();
        }
    }

    neuron* createNonInnerNeuron(NeuronTypes type) {
        switch (type) {
            case NeuronTypes::LEFT_EYE:
                return new leftEye();
            case NeuronTypes::RIGHT_EYE:
                return new rightEye();
            case NeuronTypes::TOP_EYE:
                return new topEye();
            case NeuronTypes::BOTTOM_EYE:
                return new bottomEye();
            case NeuronTypes::GO_LEFT:
                return new goLeft(); // Add more cases for other neuron types
            case NeuronTypes::GO_RIGHT:
                return new goRight();
            case NeuronTypes::GO_UP:
                return new goUp();
            case NeuronTypes::GO_DOWN:
                return new goDown();
            case NeuronTypes::KILL:
                return new kill();
            // Add more cases for other neuron types
            default:
                throw invalid_argument("Neuron type is unknown!");
        }
    }

    void decodeGenomesAndSendInputs(){

        /*
            Genome: 0-0011111-0-0000110-01111011010101110100010111000110
                    ^SourceID ^DestID   ^Weight

            Decode each genome and send the output to the destination neuron
        */

        // Decode all nonInnerNeurons
        for(int i=0; i < NumberOfNeuronTypes; i++){

            auto SOURCE = DNA[i].getSource();
            auto DESTINATION = DNA[i].getDestination();
            double weight = DNA[i].getWeight();

            neuron* SOURCE_NEURON;
            neuron* DESTINATION_NEURON;

                    // Decode SOURCE

            // Inner Neuron
            if(SOURCE.first == true){   
                unsigned int sourceID = SOURCE.second % maxInnerNeuron;
                SOURCE_NEURON = innerNeurons[sourceID];
            }

            // Non-Inner Neuron
            else if(SOURCE.first == false){
                unsigned int sourceID = SOURCE.second % NumberOfNeuronTypes;
                SOURCE_NEURON = nonInnerNeurons[sourceID];
            }

                    // Decode DESTINATION
            
            // Inner Neuron
            if(DESTINATION.first == true){
                unsigned int destinationID = DESTINATION.second % maxInnerNeuron;
                DESTINATION_NEURON = innerNeurons[destinationID];
            }

            // Non-Inner Neuron
            else if(DESTINATION.first == false){
                unsigned int destinationID = DESTINATION.second % NumberOfNeuronTypes;
                DESTINATION_NEURON = nonInnerNeurons[destinationID];
            }

            // Send output to destination neuron
            DESTINATION_NEURON->accumulateInput(SOURCE_NEURON->getOutput() * weight);    
        }

        // Finally calculate output of all neurons
        for(int i=0; i < NumberOfNeuronTypes; i++){
            nonInnerNeurons[i]->calculateOutput();
        }
        for(int i=0; i < maxInnerNeuron; i++){
            innerNeurons[i]->calculateOutput();
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
            else if(SOURCE.first == false){
                // Create INNER NEURON

            }
        }
    }

    ~NN(){
        // Delete all dynamically allocated neurons
        for(int i=0; i < NumberOfNeuronTypes+maxInnerNeuron; delete nonInnerNeurons[i++]);
        for(int i=0; i < maxInnerNeuron; delete innerNeurons[i++]);
    }
};

struct creature{

    NN brain;
    static const char symbol = 'o';
    string color;

    pair<int, int> coord;

    creature(){
        
        // Initialize random color
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distribution(0, colorMap.size() - 1);
        color = next(begin(colorMap), distribution(gen))->second;

    }


    void init(int row, int col){
        brain.setPTR(coord);

        // Set coordinates
        coord.first = row;
        coord.second = col;
    }

    void reproduce(creature &A){
        // Create new creature
    }

    void setCoordiantes(const pair<int, int> &coord){
        this->coord = coord;
    }

    void action(){
        brain.action();
    }

    void randomize(){
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distribution(0, colorMap.size() - 1);
        color = next(begin(colorMap), distribution(gen))->second;
    }

    Action randomAction() {
        return static_cast<Action>(getRandom(0, 6));
    }

};
