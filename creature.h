#include <iostream>
#include "neuron.h"

using namespace std;

static const int ID_SIZE = 7;          
static const int WEIGHT_SIZE = 32;     
static const int DNA_SIZE = (1+ID_SIZE)*2 + WEIGHT_SIZE;

struct genome{

    // Genome is a string of 0's and 1's randomly string
    string DNA;

    /*
        Genome:         0-0011111-0-0000110-01111011010101110100010111000110
                        ^SourceID ^DestID   ^Weight

        SourceID:       1~0000000 // First bit: isInner, Rest: Which source neuron is this?
        DestinationID:  1~0000000 // First bit: isInner, Rest: Which destination neuron is this?
    */

    genome(){
        // Initialize DNA randomly
        DNA = "";
        for(int i=0; i < DNA_SIZE; i++){
            DNA += to_string(getRandom(0, 1));
        }

        // TEST -  CHANGE WEIGHT TO LOWER NUMBER - DELETE THIS LATER !!!
        // DNA.replace(16, 32, "00000000000000000000000000000010");
        
    }

    /*
        Genome:         0-0011111-0-0000110-01111011010101110100010111000110
                        ^SourceID ^DestID   ^Weight
                        ^0        ^8        ^16
       
        SourceID:       1~0000000 // First bit: isInner, Rest: Which source neuron is this?
        DestinationID:  1~0000000 // First bit: isInner, Rest: Which destination neuron is this?

    */

    pair<bool, unsigned int> getSource() const {
        return make_pair(charToBool(DNA[0]), stringToUnsigned(DNA, 1, ID_SIZE));
    }

    pair<bool, unsigned int> getDestination() const {
        return make_pair(charToBool(DNA[1+ID_SIZE]), stringToUnsigned(DNA, 2+ID_SIZE, ID_SIZE));
    }

    inline unsigned int getWeight() const {
        return stringToUnsigned(DNA, 2+(ID_SIZE*2), WEIGHT_SIZE);
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

    inline void print() const {
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

    inline void printCode() const {
        // Print Genome coded
        cout << "Coded Genome:\t\t";
        for(int i=0; i < DNA_SIZE; i++){
            if(i == 0 || i == 8){cout << BOLD_RED_TEXT;}
            else if(i > 0 && i < 8 || i > 8 && i < 16){cout << BOLD_YELLOW_TEXT;}
            else if(i >= 16 && i < 48){cout << BOLD_GREEN_TEXT;}
            else{cout << RESET_TEXT;}
            cout << DNA[i] << RESET_TEXT;
        }
        cout << endl;
    }

};

struct NN{

    neuron* nonInnerNeurons[NumberOfNeuronTypes];       // All types of neurons and X number of inner neuron already initialized
    neuron* innerNeurons[maxInnerNeuron];               // Inner neurons are created dynamically
    genome DNA[maxConnection];                          // DNA of the creature
    pair<int, int> *coord;                              // Pointer to the coordinates of the creature
    
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
        for(int i=0; i < NumberOfNeuronTypes; nonInnerNeurons[i] = nn.nonInnerNeurons[i++]);
        for(int i=0; i < maxInnerNeuron; innerNeurons[i] = nn.innerNeurons[i++]);
        for(int i=0; i < maxConnection; DNA[i] = nn.DNA[i++]);
    }

    void setCreature(creature* owner){
        // Set owner of the NN
        for(int i=0; i < NumberOfNeuronTypes; nonInnerNeurons[i++]->owner = owner);
        for(int i=0; i < maxInnerNeuron; innerNeurons[i++]->owner = owner);
    }

    void setPTR(pair<int, int> &coord){
        this->coord = &coord;

        // Set pointer to coord for all neurons
        for(int i=0; i < NumberOfNeuronTypes; nonInnerNeurons[i++]->setPTR(coord));
        for(int i=0; i < maxInnerNeuron; innerNeurons[i++]->setPTR(coord));
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

    neuron* getNeuron(const pair<bool, unsigned int> aNeuron) const {
        neuron* aNeuron_NEURON = nullptr;

        // Inner Neuron
        if(aNeuron.first == true){   
            unsigned int aNeuronID = aNeuron.second % maxInnerNeuron;
            aNeuron_NEURON = innerNeurons[aNeuronID];
        }

        // Non-Inner Neuron
        else if(aNeuron.first == false){
            unsigned int aNeuronID = aNeuron.second % NumberOfNeuronTypes;
            aNeuron_NEURON = nonInnerNeurons[aNeuronID];
        }

        return aNeuron_NEURON;
    }

    void decodeGenomesAndSendInputs(){

        /*
            Genome: 0-0011111-0-0000110-01111011010101110100010111000110
                    ^SourceID ^DestID   ^Weight

            Decode each genome and send the output to the destination neuron
        */

        // Decode all nonInnerNeurons
        for(int i=0; i < NumberOfNeuronTypes; i++){

            pair<bool, unsigned int> SOURCE = DNA[i].getSource();
            pair<bool, unsigned int> DESTINATION = DNA[i].getDestination();
            double weight = DNA[i].getWeight();

            // Decode genome
            neuron* SOURCE_NEURON = getNeuron(SOURCE);
            neuron* DESTINATION_NEURON = getNeuron(DESTINATION);

            // Send output to destination neuron
            DESTINATION_NEURON->accumulateInput(SOURCE_NEURON->getOutput() * weight);    
        }

        // Finally calculate output of all neurons
        for(int i=0; i < NumberOfNeuronTypes; nonInnerNeurons[i++]->calculateOutput());
        for(int i=0; i < maxInnerNeuron; innerNeurons[i++]->calculateOutput());
    }

    inline void clearNeurons(){
        // Clear all neurons
        for(int i=0; i < NumberOfNeuronTypes; nonInnerNeurons[i++]->reset());
        for(int i=0; i < maxInnerNeuron; innerNeurons[i++]->reset());
    }

    inline void fire(){
        for(int i=0; i < NumberOfNeuronTypes; nonInnerNeurons[i++]->conditionallyDo());
        for(int i=0; i < maxInnerNeuron; innerNeurons[i++]->conditionallyDo());
    }

    void action(){
        clearNeurons();
        decodeGenomesAndSendInputs();
        fire();

    }

    void printNeuronConnections() const {
        
        cout << string(80, '-') << endl;
        cout << YELLOW_TEXT;
        cout << setw(20) << left << "Source"
        << setw(20) << left << "Destination"
        << setw(20) << left << "Weight" 
        << setw(20) << left << "Output"
        << endl;
        cout << RESET_TEXT;

        for(int i=0; i < maxConnection; i++){
            // cout << maxConnection << " - " << i << " - " << NumberOfNeuronTypes << " - " << maxInnerNeuron << endl;
            pair<bool, unsigned int> SOURCE = DNA[i].getSource();
            pair<bool, unsigned int> DESTINATION = DNA[i].getDestination();
            cout << setw(20) << left << typeid(*getNeuron(SOURCE)).name() 
            << setw(20) << left << typeid(*getNeuron(DESTINATION)).name() 
            << setw(20) << left << DNA[i].getWeight()
            << setw(20) << left << setprecision(10) << getNeuron(SOURCE)->getOutput(); // Set the precision to 10
            cout << endl;
        }
        cout << string(80, '-') << endl;
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
            // cout << "Genome:\t\t\t" << i << endl;
            DNA[i].printCode();
            // cout << endl;
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
    bool isChoosen = false;
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

    void setCreature(){
        brain.setCreature(this);
    }

    void initCoordinates(int row, int col){
        // Set coordinates
        coord.first = row;
        coord.second = col;
        brain.setPTR(coord);   
    }

    void reproduceWith(creature &A){
        // Create new creature
        creature* B = new creature();
        // Gets some genomes from parent A and some from parent B
        for(int i=0; i < maxConnection; i++){
            if(getRandom(0, 1) == 0){
                B->brain.DNA[i] = A.brain.DNA[i];
            }
            else{
                B->brain.DNA[i] = brain.DNA[i];
            }
        }

        // Mutate
        for(int i=0; i < maxConnection; i++){
            if(getRandom(0, 100) < MUTATION_RATIO){
                B->brain.DNA[i].mutation();
            }
        }

    }

    creature operator*(creature &A){

        // Reproduce with A and return the new creature

    }

    void setCoordiantes(const pair<int, int> &coord){this->coord = coord;}

    void action(){brain.action();}

    void randomize(){
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distribution(0, colorMap.size() - 1);
        color = next(begin(colorMap), distribution(gen))->second;
    }

    Action randomAction() {return static_cast<Action>(getRandom(0, 6));}

    void printDNA() const {
        cout << BLUE_TEXT << "Creature:\t\t" << this << RESET_TEXT << endl;
        brain.printDNA();
    }

    void printNeuronConnections() const {
        cout << endl << BLUE_TEXT << setw(20) << left << "Creature:" << setw(20) << left << this << RESET_TEXT << endl;
        brain.printNeuronConnections();
    }

    void print() const {
        cout << BLUE_TEXT << "Creature:\t\t" << this << RESET_TEXT << endl;
        cout << "Color:\t\t\t" << color << endl;
        cout << "Coordinates:\t\t" << coord.first << ", " << coord.second << endl;
        brain.printNeuronConnections();

        cout << endl;
    }

    ~creature(){}
};
