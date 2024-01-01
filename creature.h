#include <iostream>

#include "neuron.h"

using namespace std;

static const int ID_SIZE = 7;          
static const int WEIGHT_SIZE = 32;     
static const int DNA_SIZE = (1+ID_SIZE)*2 + WEIGHT_SIZE;

struct genome{

    bool DNA[DNA_SIZE];

    /*
        Genome:         0-0011111-0-0000110-01111011010101110100010111000110
                        ^SourceID ^DestID   ^Weight

        SourceID:       1~0000000 // First bit: isInner, Rest: Which source neuron is this?
        DestinationID:  1~0000000 // First bit: isInner, Rest: Which destination neuron is this?
    */

    genome(){
        // Initialize DNA randomly
        for(int i=0; i < DNA_SIZE; DNA[i++] = rand()%2);
    }

    pair<bool, unsigned int> getSource() const {
        return make_pair(DNA[0], boolArrayToUnsigned(&DNA[1], ID_SIZE));
    }

    pair<bool, unsigned int> getDestination() const {
        return make_pair(DNA[1+ID_SIZE], boolArrayToUnsigned(&DNA[2+ID_SIZE], ID_SIZE));
    }

    unsigned int getWeight() const {
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

    void print() const {
        // Print Genome coded and decoded
        cout << "Coded Genome:\t\t";
        for(int i=0; i < DNA_SIZE; i++){
            if(i == 0 || i == 8){cout << BOLD_RED_TEXT;}
            else if(i > 0 && i < 8 || i > 8 && i < 16){cout << BOLD_YELLOW_TEXT;}
            else if(i >= 16 && i < 48){cout << BOLD_GREEN_TEXT;}
            else{cout << RESET_TEXT;}
            cout << DNA[i] << RESET_TEXT;
        }
        cout << endl;

        auto SOURCE = getSource();
        auto DESTINATION = getDestination();

        cout << "Source ID:\t\t" << SOURCE.second << "\t- isInner: " << SOURCE.first <<endl;
        cout << "Destination ID:\t\t" << DESTINATION.second << "\t- isInner: " << DESTINATION.first << endl;
        cout << "Connection Weight\t" << getWeight() << endl;
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
            nonInnerNeurons[i]->setPTR(*coord);
        }
        // Initialize innerNeurons
        for(int i=0; i < maxInnerNeuron; i++){
            innerNeurons[i] = new inner();
            innerNeurons[i]->setPTR(*coord);
        }
    }

    NN(NN &nn){
        // Copy Constructor
        for(int i=0; i < NumberOfNeuronTypes; i++){
            nonInnerNeurons[i] = nn.nonInnerNeurons[i];
        }
        for(int i=0; i < maxInnerNeuron; i++){
            innerNeurons[i] = nn.innerNeurons[i];
        }
        for(int i=0; i < maxConnection; i++){
            DNA[i] = nn.DNA[i];
        }
    }

    void setPTR(pair<int, int> &coord){
        this->coord = &coord;

        // Set pointer to coord for all neurons
        for(int i=0; i < NumberOfNeuronTypes; i++){
            nonInnerNeurons[i]->setPTR(coord);
        }
        for(int i=0; i < maxInnerNeuron; i++){
            innerNeurons[i]->setPTR(coord);
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

    void action(){
        // Decode genomes and send inputs to neurons
        decodeGenomesAndSendInputs();
        // Do actions based on output of neurons
        for(int i=0; i < NumberOfNeuronTypes; i++){           // !!!!!!!!!! SEGMENT FAULT HERE !!!!!!!!!!!!!!!
            nonInnerNeurons[i]->conditionallyDo();
            // nonInnerNeurons[i]->print();
        }
        for(int i=0; i < maxInnerNeuron; i++){
            innerNeurons[i]->conditionallyDo();
            // innerNeurons[i]->print();
        }
    }

    /*
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
    }*/

    void printDNA() const {
        for(int i=0; i < maxConnection; i++){
            cout << "Genome:\t\t\t" << i << endl;
            DNA[i].print();
            cout << endl;
        }
    }

    ~NN(){
        // Delete all dynamically allocated neurons
        for(int i=0; i < NumberOfNeuronTypes; delete nonInnerNeurons[i++]);
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

    creature(creature &creature){
        // Copy Constructor
        this->brain = creature.brain;
        this->color = creature.color;
        this->coord = creature.coord;
    }

    void initCoordinates(int row, int col){
        // Set coordinates
        coord.first = row;
        coord.second = col;

        brain.setPTR(coord);        
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

    void printDNA() const {
        cout << BLUE_TEXT << "Creature:\t\t" << this << RESET_TEXT << endl;
        brain.printDNA();
    }

    ~creature(){}
};
