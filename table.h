#include <iostream>
#include "creature.h"

using namespace std;

#define pop 0.01         // Population density
#define sleep 10       // Miliseconds to sleep

struct dummy{
    char symbol;

    dummy(){
        symbol = (getRandom(0, 5)) + 'a';
    }

    void randomize(){
        symbol = (getRandom(0, 5)) + 'a';
    }
};

class table{

    private:

        enum Reproduce{
            RIGHT,      
            LEFT,
            TOP,
            BOTTOM,
            CENTER,
            CORNERS,
            ALL,
            SIDES,
            TOP_BOTTOM,
            LEFT_RIGHT
        };

        vector<vector<creature*>> mat;
        vector<creature*> creatures;
        unsigned int populationSize;
        creature* randomCreature;
        unsigned int iteration = 1;
        unsigned int generationNumber = 1;

        vector<creature*> reproducers;

        void randomize(){
            for(int i=0; i < creatures.size(); creatures.at(i++)->randomize());

            /*
                // Randomize all matrix elements if exits
                for(int i=0; i < mat.size(); i++){
                    for(int j=0; j < mat.at(0).size(); j++){
                        if(mat.at(i).at(j) != nullptr){mat.at(i).at(j)->randomize();}
                    }
                }
            */
        }

        void print() const { // LAST COLUMN IS WRONGLY PRINTED !!!!!!!!!!!!
            // Print whole board
            short gap = 1;
            if (mat.empty() || mat.at(0).empty()) {
                throw invalid_argument("Matrix does not exist!\nCannot print.");
            }

            cout << string(mat[0].size()*(gap+1) + 1, '|') << endl;
            for (int i = 0; i < mat.size(); i++) {
                cout << "|";
                for (int j = 0; j < mat.at(0).size(); j++) {
                    if (mat.at(i).at(j) == nullptr) {cout << setw(gap) << " ";} 
                    else {cout << mat.at(i).at(j)->color << setw(gap) << mat.at(i).at(j)->symbol << RESET_TEXT;}
                    if(j != mat.at(0).size()-1){cout << " ";}
                }
                cout << "|" << endl;
            }
            cout << string(mat[0].size()*(gap+1) + 1, '|') << endl;
        }

        void printInfo() {
            cout << "\tPopulation Size:\t" << populationSize << endl;
            cout << "\tCapacity:\t\t" << mat.size()*mat.at(0).size() << endl;
            cout << "\tIteration:\t\t" << iteration++ << endl;
            cout << "\tGeneration:\t\t" << generationNumber << endl << endl;
        }

        void clearScreen() const {cout << "\033[H\033[J";}

        void putRandomCreature(){
            // Put a random creature in a random position, if place is occupied find another place
            if(mat.empty()){throw invalid_argument("Table matrix is empty!");}
            int row = getRandom(0, mat.size()-1);
            int col = getRandom(0, mat.at(0).size()-1);
            while(mat.at(row).at(col) != nullptr){
                row = getRandom(0, mat.size()-1);
                col = getRandom(0, mat.at(0).size()-1);
            }
            creature* A = new creature();
            creatures.push_back(A);
            A->initCoordinates(row, col);
            mat.at(row).at(col) = A;
            populationSize++;
        }

        void putCreature(creature* A){
            // Put a creature in a random position, if place is occupied find another place
            if(mat.empty()){throw invalid_argument("Table matrix is empty!");}
            int row = getRandom(0, mat.size()-1);
            int col = getRandom(0, mat.at(0).size()-1);
            while(mat.at(row).at(col) != nullptr){
                row = getRandom(0, mat.size()-1);
                col = getRandom(0, mat.at(0).size()-1);
            }
            creatures.push_back(A);
            A->initCoordinates(row, col);
            mat.at(row).at(col) = A;
            populationSize++;
        }

    public:

        table(unsigned int const individualNumber, unsigned int const row, unsigned int const col){

            // Initialize pointer to creatureTable
            creatureTable = &mat;

            // Initialize matrix with given row and col numbers
             mat.resize(row, vector<creature*>(col, nullptr));

            // Constructor
            if(row*col < individualNumber){throw invalid_argument("individualNumber is invalid!");};
            populationSize = 0;
            for(int i=0; i < row; i++){
                for(int j=0; j < col; j++){
                    // If possiblitly is met put a random creature in the matrix
                    if(pop * 100 >= getRandom(1, 100)){
                        putRandomCreature();
                    }
                }
            }

            // Select a random creature
            this->randomCreature = creatures.at(getRandom(0, creatures.size()-1));
            this->randomCreature->isChoosen = true;
            this->randomCreature->setCreature();
            this->randomCreature->symbol = 'X';
            this->randomCreature->color = BOLD_WHITE_TEXT;
        }

        table(vector<vector<creature*>> &mat){this->mat = mat;}

        table(table &table){
            // Copy Constructor
            this->mat = table.mat;
            this->populationSize = table.populationSize;
        }

        inline void chooseReproducers(const Reproduce where){
            // Right half of the table reproduces
            switch (where){

                case RIGHT:
                    // Right-half of the screen will reproduce
                    for(int i=0; i < mat.size(); i++){
                        for(int j=mat.at(0).size()/2; j < mat.at(0).size(); j++){
                            if(mat.at(i).at(j) != nullptr){
                                reproducers.push_back(mat.at(i).at(j));
                            }
                        }
                    }
                    break;
                
                case LEFT:
                    // Left-half of the screen will reproduce
                    for(int i=0; i < mat.size(); i++){
                        for(int j=0; j < mat.at(0).size()/2; j++){
                            if(mat.at(i).at(j) != nullptr){
                                reproducers.push_back(mat.at(i).at(j));
                            }
                        }
                    }
                    break;

                case TOP:
                    // Top-half of the screen will reproduce
                    for(int i=0; i < mat.size()/2; i++){
                        for(int j=0; j < mat.at(0).size(); j++){
                            if(mat.at(i).at(j) != nullptr){
                                reproducers.push_back(mat.at(i).at(j));
                            }
                        }
                    }
                    break;

                case BOTTOM:
                    // Bottom-half of the screen will reproduce
                    for(int i=mat.size()/2; i < mat.size(); i++){
                        for(int j=0; j < mat.at(0).size(); j++){
                            if(mat.at(i).at(j) != nullptr){
                                reproducers.push_back(mat.at(i).at(j));
                            }
                        }
                    }
                    break;

                case CENTER:
                    // Center of the screen will reproduce
                    for(int i=mat.size()/4; i < mat.size()*3/4; i++){
                        for(int j=mat.at(0).size()/4; j < mat.at(0).size()*3/4; j++){
                            if(mat.at(i).at(j) != nullptr){
                                reproducers.push_back(mat.at(i).at(j));
                            }
                        }
                    }
                    break;

                case CORNERS:
                    // Corners of the screen will reproduce
                    for(int i=0; i < mat.size(); i++){
                        for(int j=0; j < mat.at(0).size(); j++){
                            if(mat.at(i).at(j) != nullptr){
                                if(i == 0 && j == 0){reproducers.push_back(mat.at(i).at(j));}
                                else if(i == 0 && j == mat.at(0).size()-1){reproducers.push_back(mat.at(i).at(j));}
                                else if(i == mat.size()-1 && j == 0){reproducers.push_back(mat.at(i).at(j));}
                                else if(i == mat.size()-1 && j == mat.at(0).size()-1){reproducers.push_back(mat.at(i).at(j));}
                            }
                        }
                    }
                    break;

                case ALL:
                    // All of the screen will reproduce
                    for(int i=0; i < mat.size(); i++){
                        for(int j=0; j < mat.at(0).size(); j++){
                            if(mat.at(i).at(j) != nullptr){
                                reproducers.push_back(mat.at(i).at(j));
                            }
                        }
                    }
                    break;
                
                case SIDES:
                    // Left 2 col and right 2 col of the screen will reproduce, if screen is smaller than 4 col, throw exception
                    if(mat.at(0).size() < 4){throw invalid_argument("Cannot choose reproduces, screen is smaller than 4 col!");}

                    // Left 2 col
                    for(int i=0; i < mat.size(); i++){
                        for(int j=0; j < 2; j++){
                            if(mat.at(i).at(j) != nullptr){
                                reproducers.push_back(mat.at(i).at(j));
                            }
                        }
                    }
                    
                    // Right 2 col
                    for(int i=0; i < mat.size(); i++){
                        for(int j=mat.at(0).size()-2; j < mat.at(0).size(); j++){
                            if(mat.at(i).at(j) != nullptr){
                                reproducers.push_back(mat.at(i).at(j));
                            }
                        }
                    }
                    break;

                case TOP_BOTTOM:
                    // Top 2 row and bottom 2 row of the screen will reproduce, if screen is smaller than 4 row, throw exception
                    if(mat.size() < 4){throw invalid_argument("Cannot choose reproduces, screen is smaller than 4 row!");}

                    // Top 2 row
                    for(int i=0; i < 2; i++){
                        for(int j=0; j < mat.at(0).size(); j++){
                            if(mat.at(i).at(j) != nullptr){
                                reproducers.push_back(mat.at(i).at(j));
                            }
                        }
                    }

                    // Bottom 2 row
                    for(int i=mat.size()-2; i < mat.size(); i++){
                        for(int j=0; j < mat.at(0).size(); j++){
                            if(mat.at(i).at(j) != nullptr){
                                reproducers.push_back(mat.at(i).at(j));
                            }
                        }
                    }
                    break;
                
                default:
                    break;
            }
        }

        inline void reproduce(){

            // Set matrix to nullptr
            for(int i=0; i < mat.size(); i++){
                for(int j=0; j < mat.at(0).size(); j++){
                    mat.at(i).at(j) = nullptr;
                }
            }
            
            // Shuffle the reproducers
            random_device rd;
            mt19937 g(rd());
            shuffle(reproducers.begin(), reproducers.end(), g);

            // Set population counter to 0
            populationSize = 0;

            bool flag = true;

            // Reproduce in pairs
            for(int i=0; i < reproducers.size(); i+=2){

                creature* A = reproducers.at(i);
                creature* B = reproducers.at(i+1);

                if(A == nullptr || B == nullptr){throw invalid_argument("Cannot reproduce, one of the creatures is nullptr!");}

                // Create a new creature every 2 parents will create 2 children
                creature* C = A->reproduceWith(B);
                creature* D = B->reproduceWith(A);

                if(flag){   // Select individual to monitor
                    this->randomCreature = C;
                    this->randomCreature->isChoosen = true;
                    this->randomCreature->setCreature();
                    this->randomCreature->symbol = 'X';
                    this->randomCreature->color = BOLD_WHITE_TEXT;
                    flag = false;
                }

                // Delete the parents
                delete A;
                delete B;

                // Add the children to the next generation
                putCreature(C);
                putCreature(D);

                populationSize += 2;
            }
            
            reproducers.clear();
        }

        inline bool isValid(int row, int col) {
            // Check if the given coordinates are valid
            if (row < 0 || row >= mat.size() || col < 0 || col >= mat.at(0).size()) {
                return false; // index out of range
            }
            if (mat.at(row).at(col) != nullptr) {
                return false; // not empty
            }
            return true;
        }

        inline void update(){
            for(int i=0; i < creatures.size(); creatures.at(i++)->action());
        }

        inline void screen(){
            clearScreen();                
            printInfo();
            print();
            update();
            randomCreature->printNeuronConnections();
        }

        inline void screen(int loop, int generation){
            clearScreen();
            if(mat.empty() || mat[0].empty()){throw invalid_argument("Matrix does not exist!\nCannot print.");}

            for(int i=0; i < generation; i++){
                for(int i=0; i < loop; i++){
                    screen();
                    this_thread::sleep_for(chrono::milliseconds(sleep)); // THIS DOES NOT WORK ON WINDOWS
                }
                clearScreen();
                chooseReproducers(ALL);
                reproduce();
                iteration = 1;
                generationNumber++;
            }
        }

        ~table() {
            // Destructor
            for (int i = 0; i < mat.size(); ++i) {
                for (int j = 0; j < mat[i].size(); ++j) {
                    delete mat[i][j];
                    mat[i][j] = nullptr;
                }
            }
        }
};