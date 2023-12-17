#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <iomanip> 
#include <algorithm> 
#include "evo.h"

using namespace std;

#define pop 0.01
#define stepTime 200   // FPS by milisecond

struct dummy{
    char symbol;

    dummy(){
        symbol = (getRandom(0, 5)) + 'a';
    }

    void randomize(){
        symbol = (getRandom(0, 5)) + 'a';
    }
};

template <typename T>
/*
Needed properties:

->symbol: char
->color: string
*/
class table{

    private:

        vector<vector<T*>> mat;

        void randomize(){
            for(int i=0; i < mat.size(); i++){
                for(int j=0; j < mat.at(0).size(); j++){
                    mat.at(i).at(j)->randomize();
                }
            }
        }

        void print() const {
            short gap = 2;
            if (mat.empty() || mat.at(0).empty()) {
                throw invalid_argument("Matrix does not exist!\nCannot print.");
            }


            // Print top border
            cout << string(mat.at(0).size() + 2 + (mat.at(0).size() + 1) * gap, '|') << endl;

            for (int i = 0; i < mat.size(); i++) {
                // Print left border
                cout << "| ";

                for (int j = 0; j < mat.at(0).size(); j++) {
                    if (mat.at(i).at(j) == nullptr) {
                        cout << setw(gap) << " ";
                    } else {
                        cout << mat.at(i).at(j)->color << setw(gap) << mat.at(i).at(j)->symbol << RESET_TEXT;
                    }

                    // Print space between elements
                    cout << " ";
                }

                // Print right border
                cout << " |" << endl;
            }

            // Print bottom border
            cout << string(mat.at(0).size() + 2 + (mat.at(0).size() + 1) * gap, '|') << endl;
        }



        void clearScreen() const {cout << "\033[H\033[J";}

    public:

        table(unsigned int row, unsigned int col){
            for(int i=0; i < row; i++){
                vector<T*> arow;
                for(int j=0; j < col; j++){
                    T* A = new T();
                    arow.push_back(A);
                }
                mat.push_back(arow);
            }
        }

        table(unsigned int const individualNumber, unsigned int const row, unsigned int const col){

            if(row*col < individualNumber){throw invalid_argument("individualNumber is invalid!");};

            int count=0;
            for(int i=0; i < row; i++){
                vector<T*> arow;
                for(int j=0; j < col; j++){
                    if(pop * 100 >= getRandom(1, 100)){
                        T* A = new T();
                        arow.push_back(A);                        
                    }
                    else{
                        arow.push_back(nullptr);
                    }
                }
                mat.push_back(arow);
            }
        }

        table(vector<vector<T*>> &mat){
            this->mat = mat;
        }

        table(table &table){
            this->mat = table.mat;
        }

        ~table() {
            // Release memory for each object in the matrix
            for (int i = 0; i < mat.size(); ++i) {
                for (int j = 0; j < mat[i].size(); ++j) {
                    delete mat[i][j];
                    mat[i][j] = nullptr;
                }
            }
        }

        bool isValid(int row, int col) {
            if (row < 0 || row >= mat.size() || col < 0 || col >= mat.at(0).size()) {
                return false; // index out of range
            }
            if (mat.at(row).at(col) != nullptr) {
                return false; // not empty
            }
            return true;
        }


        void update(){
            for(int i=0; i < mat.size(); i++){
                for(int j=0; j < mat.at(0).size(); j++){
                    if(mat.at(i).at(j) == nullptr){continue;}
                    switch (mat.at(i).at(j)->randomMove()) {
                        case 1:
                            // Handle UP
                            if (isValid(i - 1, j)) {
                                mat.at(i - 1).at(j) = mat.at(i).at(j);
                                mat.at(i).at(j) = nullptr;
                            }
                            break;
                        case 2:
                            // Handle LEFT
                            if (isValid(i, j - 1)) {
                                mat.at(i).at(j - 1) = mat.at(i).at(j);
                                mat.at(i).at(j) = nullptr;
                            }
                            break;
                        case 3:
                            // Handle RIGHT
                            if (isValid(i, j + 1)) {
                                mat.at(i).at(j + 1) = mat.at(i).at(j);
                                mat.at(i).at(j) = nullptr;
                            }
                            break;
                        case 4:
                            // Handle DOWN
                            if (isValid(i + 1, j)) {
                                mat.at(i + 1).at(j) = mat.at(i).at(j);
                                mat.at(i).at(j) = nullptr;
                            }
                            break;
                        case 5:
                            // Handle STAY
                            break;
                        default:
                            // Handle default case
                            break;
                    }

                }
            }
        }

        void screen(unsigned int seconds) {
            clearScreen();
            if (mat.empty() || mat[0].empty()) {
                throw invalid_argument("Matrix does not exist!\nCannot print.");
            }

            auto start_time = chrono::high_resolution_clock::now();
            auto end_time = start_time + chrono::seconds(seconds);

            while (chrono::high_resolution_clock::now() < end_time) {
                clearScreen();
                print();
                this_thread::sleep_for(chrono::milliseconds(stepTime)); // Adjust sleep duration as needed
                // randomize();
                update();
            }
        }

};

int main(){

    table<creature>A(10, 50, 50);
    A.screen(15);
    
}