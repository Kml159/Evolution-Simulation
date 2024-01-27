#include <iostream>
#include "table.h"

using namespace std;

int main(){

    srand(static_cast<unsigned>(time(nullptr)));
    table A(10, 25, 25);

    A.screen(3, 700);   
    return 0;
}