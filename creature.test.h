#include <iostream>
#include <vector>
#include "creature.h"

int main() {
    // Test the creation of non-inner neurons
    NN nn;
    for (int i = 0; i < NumberOfNeuronTypes; i++) {
        neuron* n = nn.neurons[i];
        std::cout << "Non-inner neuron type: " << i << std::endl;
        // Test the output function of the neuron
        double output = n->output(0, 0, std::vector<std::vector<creature>>());
        std::cout << "Output: " << output << std::endl;
    }

    // Test the creation of inner neurons
    for (int i = NumberOfNeuronTypes; i < NumberOfNeuronTypes + maxInnerNeuron; i++) {
        neuron* n = nn.neurons[i];
        std::cout << "Inner neuron type: " << i << std::endl;
        // Test the output function of the neuron
        double output = n->output(0, 0, std::vector<std::vector<creature>>());
        std::cout << "Output: " << output << std::endl;
    }

    // Test the genome class
    genome g;
    std::cout << "Source ID: " << g.getSource().second << std::endl;
    std::cout << "Destination ID: " << g.getDestination().second << std::endl;
    std::cout << "Weight: " << g.getWeight() << std::endl;

    // Test the crossover and mutation functions of the genome
    genome g1;
    genome g2;
    g1.crossOver(g2);
    std::cout << "After crossover:" << std::endl;
    std::cout << "Source ID of g1: " << g1.getSource().second << std::endl;
    std::cout << "Source ID of g2: " << g2.getSource().second << std::endl;

    g1.mutation();
    std::cout << "After mutation:" << std::endl;
    std::cout << "Source ID of g1: " << g1.getSource().second << std::endl;

    return 0;
}