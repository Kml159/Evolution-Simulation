#include <iostream>
#include "table.h"

using namespace std;

int main(){

    srand(static_cast<unsigned>(time(nullptr)));
    table A(10, 20, 40);

    A.screen(60, 50);   

    return 0;
}