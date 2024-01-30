#include <iostream>
// include windows.h for if you are using windows
#ifdef _WIN32
#include <windows.h>
#endif

#include "creature.h"

using namespace std;

#define asd 0.1         // Population density

enum reproductionTypes{
    SURVIVALIST,
    CONST_POP,
    ELITIST
};

enum survivalSpot{
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

class simulation{

    private:

        

        vector<vector<creature*>> mat;
        vector<creature*> creatures;
        vector<creature*> reproducers;

        creature* monitoredCreature;
        
        unsigned int populationSize;
        unsigned int prevPopulationSize;
        unsigned int normalPopulationSize;
        unsigned int iteration = 1;
        unsigned int generationNumber = 1;

        double currentPopulationDensity = 0.0;
        double generationScore = 100.0;

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

        void print() const {
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
            cout << BOLD_CYAN_TEXT << setw(30) << left << "Init. Population: " + to_string(normalPopulationSize)
            << setw(30) << left <<  "Population Size: " + to_string(populationSize)
            << setw(30) << left <<  "Capacity: " + to_string(mat.size()*mat.at(0).size())
            << setw(30) << left <<  "Population Density: " + to_string(currentPopulationDensity) << endl
            << setw(30) << left <<  "Generation Score: " + to_string(generationScore)
            << setw(30) << left <<  "Iteration: " + to_string(iteration++)
            << setw(30) << left <<  "Generation: " + to_string(generationNumber)
            << setw(30) << left <<  "Reproducers: " + to_string(reproducers.size()) << RESET_TEXT << endl << endl;
        }

        void clearScreen() const {cout << "\033[H\033[J";}

        inline void putRandomCreature(){
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

        inline void putCreature(creature* A){
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
    
        inline void update(){
            for(int i=0; i < creatures.size(); creatures.at(i++)->action());
        }

        inline void clearCreaturesTable(){
            // Clear creatures table
            creatures.clear();

            // Set matrix to nullptr
            for(int i=0; i < mat.size(); i++){
                for(int j=0; j < mat.at(0).size(); j++){
                    mat.at(i).at(j) = nullptr;
                }
            }

            this->populationSize = 0;
        }

        inline void setMonitoredCreature(creature* choosen){
            if(monitoredCreature != nullptr){
                monitoredCreature->isChoosen = false;
                monitoredCreature->symbol = 'o';                
            }

            this->monitoredCreature = choosen;
            this->monitoredCreature->isChoosen = true;
            this->monitoredCreature->setCreature();
            this->monitoredCreature->symbol = 'X';
            this->monitoredCreature->color = BOLD_WHITE_TEXT;
        }
        
        inline void reproduceSurvivalist(){

            clearCreaturesTable();

            shuffle(reproducers.begin(), reproducers.end(), mt19937(random_device()()));

            bool flag = true; 

            if(reproducers.size() % 2 != 0){
                creature* A = reproducers.at(reproducers.size()-1);
                creature* childA = A->reproduceWith(reproducers.at(getRandom(0, reproducers.size()-1)));
                putCreature(childA);
                delete A;
                reproducers.pop_back();
            }

            // Reproduce in pairs
            for(int i=0; i < reproducers.size(); i+=2){

                creature* A = reproducers.at(i);
                creature* B = reproducers.at(i+1);

                if(A == nullptr || B == nullptr){throw invalid_argument("Cannot reproduce, one of the creatures is nullptr!");}

                // Create a new creature every 2 parents will create 2 children
                creature* childA = A->reproduceWith(B);
                creature* childB = B->reproduceWith(A);

                // Select individual to monitor
                if(flag){   
                    setMonitoredCreature(childA);
                    flag = false;
                }

                // Delete the parents
                delete A;
                delete B;

                // Add the children to the next generation
                putCreature(childA);
                putCreature(childB);

                populationSize += 2;
            }

            reproducers.clear();

        }

        inline void reproduceConstPop(){
            // Keep the population size constant

            clearCreaturesTable();
            shuffle(reproducers.begin(), reproducers.end(), mt19937(random_device()()));
            bool flag = true;
            if(reproducers.size() % 2 != 0){
                creature* A = reproducers.at(reproducers.size()-1);
                creature* childA = A->reproduceWith(reproducers.at(getRandom(0, reproducers.size()-1)));
                putCreature(childA);
                delete A;
                reproducers.pop_back();
            }

            // Reproduce in pairs
            for(int i=0; i < reproducers.size(); i+=2){

                creature* A = reproducers.at(i);
                creature* B = reproducers.at(i+1);

                if(A == nullptr || B == nullptr){throw invalid_argument("Cannot reproduce, one of the creatures is nullptr!");}

                // Create a new creature every 2 parents will create 2 children
                creature* childA = A->reproduceWith(B);
                creature* childB = B->reproduceWith(A);

                // Select individual to monitor
                if(flag){   
                    setMonitoredCreature(childA);
                    flag = false;
                }

                // Delete the parents
                delete A;
                delete B;

                // Add the children to the next generation
                putCreature(childA);
                putCreature(childB);

                populationSize += 2;
            }

            reproducers.clear();

            while(populationSize < normalPopulationSize){
                creature* ParentA = creatures.at(getRandom(0, creatures.size()-1));
                creature* ParentB = creatures.at(getRandom(0, creatures.size()-1));

                creature* childA = ParentA->reproduceWith(ParentB);

                putCreature(childA);
            }
        }

        inline void reproduceElitist(){
            // Parent will not die after reproduction: Elitist

            clearCreaturesTable();
            shuffle(reproducers.begin(), reproducers.end(), mt19937(random_device()()));
            bool flag = true;

            if(reproducers.size() % 2 != 0){
                creature* A = reproducers.at(reproducers.size()-1);
                creature* childA = A->reproduceWith(reproducers.at(getRandom(0, reproducers.size()-1)));
                putCreature(childA);
                delete A;
                reproducers.pop_back();
            }

            // Reproduce in pairs
            for(int i=0; i < reproducers.size(); i+=2){

                creature* A = reproducers.at(i);
                creature* B = reproducers.at(i+1);

                if(A == nullptr || B == nullptr){throw invalid_argument("Cannot reproduce, one of the creatures is nullptr!");}

                // Create a new creature every 2 parents will create 2 children
                creature* childA = A->reproduceWith(B);
                delete B;

                // Select individual to monitor
                if(flag){   
                    setMonitoredCreature(childA);
                    flag = false;
                }

                // Add the children to the next generation
                putCreature(childA);
                putCreature(A);

                populationSize += 4;
            }

            reproducers.clear();
        }

        inline void chooseReproducers(const survivalSpot where){
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
                    for(int i=0; i < creatures.size(); i++){
                        reproducers.push_back(creatures.at(i));
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

        inline void simulateGeneration(int loop, int sleep){
            for(int i=0; i < loop; i++){

                clearScreen();                
                printInfo();
                print();
                // randomCreature->printNeuronConnections();

                #ifdef _WIN32
                Sleep(sleep); // THIS WORKS ON WINDOWS
                #else
                this_thread::sleep_for(chrono::milliseconds(sleep)); // THIS DOES NOT WORK ON WINDOWS
                #endif

                update();
                this->currentPopulationDensity = (double)populationSize / (double)(mat.at(0).size()*mat.size()) * 100.0;
            }
            prevPopulationSize = populationSize;
        }

    public:

        simulation(unsigned int const individualNumber, unsigned int const row, unsigned int const col, double populationDensity = 0.2){

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
                    if(populationDensity * 100 >= getRandom(1, 100)){
                        putRandomCreature();
                    }
                }
            }

            // Select a random creature to monitor
            setMonitoredCreature(creatures.at(getRandom(0, creatures.size()-1)));

            this->normalPopulationSize = populationSize;
            this->currentPopulationDensity = (double)populationSize / (double)(row*col) * 100.0;
        }

        inline void screen(int loop, int generation, int sleep, const reproductionTypes REPRODUCTION, const survivalSpot SELECTION){
            clearScreen();
            if(mat.empty() || mat[0].empty()){throw invalid_argument("Matrix does not exist!\nCannot print.");}

            bool isAllDead = false;
            for(int i=0; i < generation; i++){
                
                simulateGeneration(loop, sleep);
                chooseReproducers(SELECTION);
                
                switch (REPRODUCTION){
                    case SURVIVALIST:
                        reproduceSurvivalist();
                        break;
                    case CONST_POP:
                        reproduceConstPop();
                        break;
                    case ELITIST:
                        reproduceElitist();
                        break;
                    default:
                        break;
                }

                // Calculate how successful the generation was
                generationScore = (double)populationSize / (double)prevPopulationSize * 100.0;

                // If there is no more creatures, terminate
                if(populationSize == 0){
                    if(isAllDead){
                        cout << RED_TEXT << "All creatures are dead!" << RESET_TEXT << endl;
                        break;
                    }
                    
                    isAllDead = true;
                }

                iteration = 1;
                generationNumber++;
            }
        }

        ~simulation() {
            // Destructor
            mat.clear();

            // Delete creatures
            for(int i=0; i < creatures.size(); i++){
                if(creatures.at(i) != nullptr){
                    delete creatures.at(i);
                    creatures.at(i) = nullptr;
                }
            }
            creatures.clear();

            // Delete reproducers
            for(int i=0; i < reproducers.size(); i++){
                if(reproducers.at(i) != nullptr){
                    delete reproducers.at(i);
                    reproducers.at(i) = nullptr;
                }
            }            

            cout << GREEN_TEXT << "Program terminated successfully!" << RESET_TEXT << endl;
        }
};