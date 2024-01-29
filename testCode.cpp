#include <iostream>
#include "table.h"

using namespace std;

/*
    - TRY ELITIST !!!
    - THERE IS A PROBLEM WITH CHOOSING THE REPRODUCERS !!!
*/

int main(){

    srand(static_cast<unsigned>(time(nullptr)));
    table A(20, 50, 90);

    A.screen(100, 60, 50);   

    return 0;
}