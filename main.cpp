#include <iostream>
#include "simulation.h"

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

    simulation A(50, 90, 0.1);

    int oneGeneration = 100;
    int numberOfGenerations = 20;
    int sleepDuration = 50;

    A.screen(oneGeneration, numberOfGenerations, sleepDuration, reproductionTypes::ELITIST, survivalSpot::RIGHT);   

    return 0;
}