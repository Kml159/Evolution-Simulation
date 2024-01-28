#include <iostream>
#include "table.h"

using namespace std;

int main(){

    srand(static_cast<unsigned>(time(nullptr)));
    table A(10, 20, 20);

    A.screen(50, 10);   
    return 0;
}