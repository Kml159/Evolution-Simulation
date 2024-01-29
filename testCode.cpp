#include <iostream>
#include "table.h"

using namespace std;

/*
    - TRY ELITIST !!!
*/

int main(){

    srand(static_cast<unsigned>(time(nullptr)));
    table A(20, 50, 90);

    A.screen(100, 60, 10);   

    return 0;
}