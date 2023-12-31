#include <vector>
#include <unordered_map>
#include "static.h"

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

struct activationFunctions {
    // Activation functions are used to determine the output of a neuron
    static double sigmoid(double x) {
        return 1.0 / (1.0 + exp(-x));
    }

    static double relu(double x) {
        return max(0.0, x);
    }
};

template <typename T>
struct neuron{
    // Neurons are the building blocks of neural networks
    // const static vector<vector<T*>> *mat;
    double bias;
    double accumulation;
    double output;
    char genome[genomeLength];

    pair<int, int> *coord;      // Pointer to the coordinates of the creature
    vector<vector<T*>> *mat;    // Pointer to the matrix

    neuron(){
        // Initialize bias randomly
        bias = getRandomDouble(-1.0, 1.0);
        accumulation = 0.0;
    }

    /*
    template <typename T>
    void unconditionallyDo(unsigned int row, unsigned int col, vector<vector<T*>> &mat);

    !????????!?!?!?!?!?!?!? 
    */

    void setPTR(const pair<int, int> &coord, const vector<vector<T*>> &mat){
        this->coord = &coord;
        this->mat = &mat;
    }

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

    bool isOutOfBounds(int row, int col){
        // If indices are out of bounds do nothing
        if(row > mat->size()-1 || row < 0 || col > mat->at(0).size()-1 || col < 0){return true;}
        return false; 
    }

    template <typename T>
    bool isOccupied(int row, int col){
        // If given indices are occupied by another creature do nothing
        if(mat->at(row).at(col) != nullptr){return true;}
        return false;
    }

    double getOutput() const {return output;}

};

// NON-INNER NEURONS

// INPUT NEURONS 
template <typename T>
struct leftEye: neuron{         // CORRECTED
    double output() const {
        // If there is a creature left of the current creature return 1.0, else return 0.0
        if(coord->first > mat->size()-1 || coord->first < 0 || coord->second > mat->at(0).size()-1 || coord->second < 0){return -1.0;}   // If there is a wall
        if(mat->at(row).at(col-1) != nullptr){return 1.0;}                                        // If there is a creature
        else{return 0.0;}
    }
};

// REST NEURON OUTPUTS FUNCTIONS ARE FALSE !!!!!!!!!!!
template <typename T>
struct rightEye: neuron{
    double output(unsigned int row, unsigned int col, const vector<vector<T*>> &mat) const {
        // If there is a creature right of the current creature return 1.0, else return 0.0
        if(row > mat.size()-1 || row < 0 || col > mat.at(0).size()-1 || col < 0){return -1.0;}   // If there is a wall
        if(mat.at(row).at(col+1) != nullptr){return 1.0;}                                        // If there is a creature
        else{return 0.0;}
    }
};

template <typename T>
struct topEye: neuron{
    double output(unsigned int row, unsigned int col, const vector<vector<T*>> &mat) const {
        // If there is a creature above the current creature return 1.0, else return 0.0
        if(row > mat.size()-1 || row < 0 || col > mat.at(0).size()-1 || col < 0){return -1.0;}   // If there is a wall
        if(mat.at(row+1).at(col) != nullptr){return 1.0;}                                        // If there is a creature
        else{return 0.0;}
    }
};

template <typename T>
struct bottomEye: neuron{
    double output(unsigned int row, unsigned int col, const vector<vector<T*>> &mat) const {
        // If there is a creature below the current creature return 1.0, else return 0.0
        if(row > mat.size()-1 || row < 0 || col > mat.at(0).size()-1 || col < 0){return -1.0;}   // If there is a wall
        if(mat.at(row-1).at(col) != nullptr){return 1.0;}                                        // If there is a creature
        else{return 0.0;}
    }
};

// MUSCLE NEURONS 
template <typename T>
struct goLeft: neuron{

    void unconditionallyDo(unsigned int row, unsigned int col, vector<vector<T*>> &mat){
        // If indices are out of bounds do nothing
        if(isOutOfBounds(unsigned int row, unsigned int col, vector<vector<T*>> &mat)){return;}
        // If there is another creature do nothing
        else if(isOccupied(unsigned int row, unsigned int col, vector<vector<T*>> &mat)){return;}
        // By swapping the creatures we are moving the current creature to the left
        swap(mat.at(row).at(col), mat.at(row).at(col-1));
    }

    void conditionallyDo(unsigned int row, unsigned int col, vector<vector<T*>> &mat){
        // If output is positive do the action
        if(output(row, col, mat) > 0){unconditionallyDo(row, col, mat);}
    }
};

template <typename T>
struct goRight: neuron{
    
    void unconditionalyDo(unsigned int row, unsigned int col, vector<vector<T*>> &mat){
        // If indices are out of bounds do nothing
        if(isOutOfBounds(unsigned int row, unsigned int col, vector<vector<T*>> &mat)){return;}
        // If there is another creature do nothing
        else if(isOccupied(unsigned int row, unsigned int col, vector<vector<T*>> &mat)){return;}
        // By swapping the creatures we are moving the current creature to the right
        swap(mat.at(row).at(col), mat.at(row).at(col+1));
    }

    void conditionallyDo(unsigned int row, unsigned int col, vector<vector<T*>> &mat){
        // If output is positive do the action
        if(output(row, col, mat) > 0){unconditionallyDo(row, col, mat);}
    }
};

template <typename T>
struct goUp: neuron{
    
    void unconditionalyDo(unsigned int row, unsigned int col, vector<vector<T*>> &mat){
        // If indices are out of bounds do nothing
        if(isOutOfBounds(unsigned int row, unsigned int col, vector<vector<T*>> &mat)){return;}
        // If there is another creature do nothing
        else if(isOccupied(unsigned int row, unsigned int col, vector<vector<T*>> &mat)){return;}
        // By swapping the creatures we are moving the current creature to the top
        swap(mat.at(row).at(col), mat.at(row+1).at(col));
    }

    void conditionallyDo(unsigned int row, unsigned int col, vector<vector<T*>> &mat){
        // If output is positive do the action
        if(output(row, col, mat) > 0){unconditionallyDo(row, col, mat);}
    }
};

template <typename T>
struct goDown: neuron{
    
    void unconditionalyDo(unsigned int row, unsigned int col, vector<vector<T*>> &mat){
        // If indices are out of bounds do nothing
        if(isOutOfBounds(unsigned int row, unsigned int col, vector<vector<T*>> &mat)){return;}
        // If there is another creature do nothing
        else if(isOccupied(unsigned int row, unsigned int col, vector<vector<T*>> &mat)){return;}
        // By swapping the creatures we are moving the current creature to the bottom
        swap(mat.at(row).at(col), mat.at(row-1).at(col));
    }

    void conditionallyDo(unsigned int row, unsigned int col, vector<vector<T*>> &mat){
        // If output is positive do the action
        if(output(row, col, mat) > 0){unconditionallyDo(row, col, mat);}
    }
};

template <typename T>
struct kill: neuron{
    double output(unsigned int row, unsigned int col, vector<vector<T*>> &mat){
        // Implement this !!!!!!!!!!!!
    }
};

// INNER NEURONS
template <typename T>
struct inner: neuron{

};

static const int ID_SIZE = 7;          
static const int WEIGHT_SIZE = 32;     
static const int DNA_SIZE = (1+ID_SIZE)*2 + WEIGHT_SIZE;

static const double CROSS_OVER_RATIO = 0.2;
static const double MUTATION_RATIO = 0.1;

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

template <typename T>
struct NN{

    neuron* nonInnerNeurons[NumberOfNeuronTypes];    // All types of neurons and X number of inner neuron already initialized
    neuron* innerNeurons[maxInnerNeuron];            // Inner neurons are created dynamically
    genome DNA[maxConnection];

    pair<int, int> *coord;      // Pointer to the coordinates of the creature
    vector<vector<T*>> *mat;    // Pointer to the matrix
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
            innerNeurons[i] = new inner<creature>();
        }

        // Set pointers of neurons
        for(int i=0; i < NumberOfNeuronTypes; i++){
            nonInnerNeurons[i]->setPTR(*coord, *mat);
        }

        for(int i=0; i < maxInnerNeuron; i++){
            innerNeurons[i]->setPTR(*coord, *mat);
        }
    }

    void setPTR(const pair<int, int> &coord, const vector<vector<T*>> &mat){
        this->coord = &coord;
        this->mat = &mat;
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

template <typename T>
struct creature{

    NN brain;
    static const char symbol = 'o';
    string color;

    pair<int, int> coord;
    vector<vector<T*>> *mat;    // Pointer to the matrix

    creature(vector<vector<T*>> &mat){
        this->mat = &mat;

        // Initialize random color
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distribution(0, colorMap.size() - 1);
        color = next(begin(colorMap), distribution(gen))->second;

        // Initialize random coordinates
        coord.first = getRandom(0, maxRow-1);
        coord.second = getRandom(0, maxCol-1);

        brain.setCoordiantes(coord);

        // Set pointer of brain
        brain.setPTR(coord, *mat);
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
