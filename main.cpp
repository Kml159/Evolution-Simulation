#include <iostream>
#include "table.h"

using namespace std;

/*
    TO DO's:

    - Write a function that calculates standard deviation of DNA begin to end of single creature
    - put creatures into vector !!!
    - use mutate
    - figure out why creatures are not moving after 1-2 generations
    - print the changes in single DNA

    DONE:

    - Print connections between neurons and what would activate them !!!

*/

int main(){

    srand(static_cast<unsigned>(time(nullptr)));
    table A(20, 50, 90);

    A.screen(100, 3, 50);   

    return 0;
}