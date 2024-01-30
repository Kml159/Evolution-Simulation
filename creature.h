#include <iostream>
#include "neuron.h"

using namespace std;

// !!! DO NOT CHANGE THIS !!!
static const int ID_SIZE = 7;          
static const int WEIGHT_SIZE = 32;     
static const int DNA_SIZE = (1+ID_SIZE)*2 + WEIGHT_SIZE;
// !!! DO NOT CHANGE THIS !!!

struct genome{

    string DNA;
    static constexpr double MUTATION_RATIO = 0.02;

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

    genome(genome &genome){
        // Copy Constructor
        this->DNA = genome.DNA;
    }

    pair<bool, unsigned int> getSource() const {
        return make_pair(charToBool(DNA[0]), stringToUnsigned(DNA, 1, ID_SIZE));
    }

    pair<bool, unsigned int> getDestination() const {
        return make_pair(charToBool(DNA[1+ID_SIZE]), stringToUnsigned(DNA, 2+ID_SIZE, ID_SIZE));
    }

    inline unsigned int getWeight() const {
        return stringToUnsigned(DNA, 2+(ID_SIZE*2), WEIGHT_SIZE);
    }

    inline genome crossoverWith(genome& other){
        // Genomes that will be crossed over are will be same indices !
        // Crossover will be part by part
        int doneIndex = 0;
        int minPartSize = 2;         // IMPORTANT !!!

        genome* A = this;
        genome* B = &other;
        genome child;

        /*
            Parent A:  0-0011111-0-0000110-01100010000101110100010111000110
            Parent B:  1-0000000-1-0000110-01111011011101100101011011000110

            Child:     0-0011111-1-0000110-01100011010101100101010111000110
        */
        
        while(true){
            if(doneIndex >= DNA_SIZE){break;}   // If all DNA is crossed over, break

            bool isFromA = maybe;    // Randomly choose from which parent to take the part
            int size = getRandom(minPartSize, DNA_SIZE - doneIndex);    // Randomly choose the size of the part

            // ParentA or ParentB -> Child
            if(isFromA){
                child.DNA.replace(doneIndex, size, A->DNA.substr(doneIndex, size));
            }
            else{
                child.DNA.replace(doneIndex, size, B->DNA.substr(doneIndex, size));
            }

            doneIndex += size;
        }

        return child;
    }

    inline void mutation(){
        // Mutate DNA
        for(int i=0; i < DNA_SIZE; i++){
            if(getRandomDouble(0.0, 1.0) < MUTATION_RATIO){
                DNA[i] = (DNA[i] == '0') ? '1' : '0';
            }
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
    bool* isChoosen;

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

    // ONLY FOR MONITORING RANDOMY SELECTED CREATURE !!!
    void setCreature(creature* owner){
        // Set owner of the NN
        for(int i=0; i < NumberOfNeuronTypes; nonInnerNeurons[i++]->owner = owner);
        for(int i=0; i < maxInnerNeuron; innerNeurons[i++]->owner = owner);
    }

    void setPTR(pair<int, int> &coord, bool* isChoosen){
        this->coord = &coord;

        // Set pointer to coord for all neurons
        for(int i=0; i < NumberOfNeuronTypes; nonInnerNeurons[i++]->setPTR(coord));
        for(int i=0; i < maxInnerNeuron; innerNeurons[i++]->setPTR(coord));

        this->isChoosen = isChoosen;
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
            case NeuronTypes::LATERAL_LOCATION:
                return new lateralLocation();
            case NeuronTypes::VERTICAL_LOCATION:
                return new verticalLocation();
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

        // Print all neuron connections
        for(int i=0; i < maxConnection; i++){
            pair<bool, unsigned int> SOURCE = DNA[i].getSource();
            pair<bool, unsigned int> DESTINATION = DNA[i].getDestination();
            
            // Avoid multiple calls to getNeuron with side effects
            auto sourceNeuron = getNeuron(SOURCE);
            auto destNeuron = getNeuron(DESTINATION);

            // Use auto to simplify the type and avoid casting
            auto sourceName = typeid(*sourceNeuron).name();
            auto destName = typeid(*destNeuron).name();

            if(sourceNeuron->getOutput() == 1.0){cout << BLUE_TEXT;}
            else if(sourceNeuron->getOutput() > 0){cout << GREEN_TEXT;}
            else if(sourceNeuron->getOutput() == -1.0){cout << RED_TEXT;}

            cout << setw(20) << left << sourceName
                << setw(20) << left << destName
                << setw(20) << left << DNA[i].getWeight()
                << setw(20) << left << setprecision(10) << sourceNeuron->getOutput();
            cout << RESET_TEXT << endl;
        }
        cout << string(80, '-') << endl;
    }

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
    char symbol = 'o';
    string color;
    bool isChoosen = false;
    bool allowed2Reproduce = false;
    pair<int, int> coord;

    int score = 0;

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
        brain.setPTR(coord, &isChoosen);   
    }

    creature* reproduceWith(creature* other){
        /*
            HOW TO REPRODUCE:

            ParentA:
                DNA0 = 0111010101010101010101010101010101010101010101010101010101010101
                DNA1 = 1101111010101010101010101010101010101010101010101010101010101010
                DNA2 = 1110010010100110111010000101001101010011010100101010111110000000

            ParentB:
                DNA0 = 1010010011110111101011101001101010101111100101011011101111110000
                DNA1 = 1001010111101010101110100001000111111101111011101001001010001001
                DNA2 = 1001111101001001001001010010100100101001100100100101010010101010

            Child:
                DNA0 <- ParentA.DNA0.crossOver(ParentB.DNA0)
                DNA1 <- ParentA.DNA1.crossOver(ParentB.DNA1)
                DNA2 <- ParentA.DNA2.crossOver(ParentB.DNA2)
        */

        creature* child = new creature();
        
        // Crossover
        for(int i=0; i < maxConnection; i++){
            child->brain.DNA[i] = this->brain.DNA[i].crossoverWith(other->brain.DNA[i]);
        }

        // Mutation
        for(int i=0; i < maxConnection; i++){
            child->brain.DNA[i].mutation();
        }

        return child;
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
        // Print coordinates
        cout << setw(20) << left << "Coordinates (RXC):" << coord.first << " X " << coord.second 
        // Print all table size
        << "\t\tTable Size: " << creatureTable->size() << " X " << creatureTable->at(0).size() << RESET_TEXT
        << endl;
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
