#include <vector>
#include <unordered_map>
#include "func.h"

using namespace std;

#define maxConnection 8
#define genomeLength 100

struct neuron{
    pair<neuron*, double> connection[maxConnection];
    double bias;
    double output;
};

struct NN{

};

struct creature{

    pair<int, int> coord;
    bool genome[genomeLength];
    NN brain;
    char symbol;
    string color;

    creature(){
        symbol = 'o'; //getRandom(1, 16) + '0';
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distribution(0, colorMap.size() - 1);

        color = next(begin(colorMap), distribution(gen))->second;
    }

    void randomize(){
        symbol = (getRandom(0, 5)) + 'a';
    }

    unsigned short randomMove() {
        /*
            UP = 1,
            LEFT = 2,
            RIGHT = 3,
            DOWN = 4
        */
        return (unsigned short)getRandom(1, 5);
    }
};
