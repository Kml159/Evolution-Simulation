#include <iostream>
#include "simulation.h"

using namespace std;

/*
    TO DO's:

    - Cross over function is problematic. Irrelevant connections might be crossed over. FIX THIS !!!!
    - Destination neurons should only be muscle or inner neurons. FIX THIS !!!!
    - Bias is not updated. FIX THIS !!!!

    DONE:

    - Print connections between neurons and what would activate them !!!

*/

/**
 * @brief The main function of the program.
 * 
 * @param oneGeneration         The number of steps in one generation.
 * @param numberOfGenerations   The number of generations.
 * @param sleepDuration         The duration of sleep between each step.
 * @param reproductionType      How the creatures will reproduce.
 * @param survivalSpot          Where the creatures will be placed after each generation.
 * 
 * @author Kml159.
 * @date 30.01.2024.
 * @return An integer representing the exit status of the program.
 */

int main(){

    srand(static_cast<unsigned>(time(nullptr)));

    simulation A(50, 90, 0.7);

    int oneGeneration =         100;
    int numberOfGenerations =   10;
    int sleepDuration =         50;

    A.screen(oneGeneration, numberOfGenerations, sleepDuration, reproductionTypes::SURVIVALIST, survivalSpot::RIGHT);   

    return 0;
}