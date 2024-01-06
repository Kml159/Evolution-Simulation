#include <iostream>
#include "table.h"

using namespace std;

/*
    TO DO's:

    - Write a function that calculates standard deviation of DNA begin to end of single creature
    - put creatures into vector !!!
    - use mutate
    - figure out why creatures are not moving after 1-2 generations

    DONE:

    - Print connections between neurons and what would activate them !!!

*/

int main(){

    srand(static_cast<unsigned>(time(nullptr)));
    table A(10, 50, 50);

    A.screen(10, 'Y');   
}