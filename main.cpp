#include <iostream>
#include "table.h"

using namespace std;

/*
    TO DO's:

    - Find what casuses the segmentation fault and fix it (its somewhere in the muscle neurons)

*/

int main(){

    srand(static_cast<unsigned>(time(nullptr)));
    table A(10, 50, 50);

    A.screen(15);
    
}