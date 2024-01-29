#include <iostream>
#include "table.h"

using namespace std;

/*
    ERROR:
        - In next generation, neurons are not created (this = nullptr)
        - Somehow program still using parents in next generation
            - exception in converting dna to int or otherwise
*/

int main(){

    srand(static_cast<unsigned>(time(nullptr)));
    table A(20, 50, 90);

    A.screen(100, 60, 10);   

    return 0;
}