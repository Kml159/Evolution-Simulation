# Evolution of Creatures - Simulation
## Introduction
This project is a simulation of the evolution of a population of creatures. The creatures are represented by a neural network. The neural network is trained using a genetic algorithm. The creatures are controlled by the neural network. The creatures are placed in a 2D world. 

## The World
The world is a 2D grid. It contains creatures

## The Creatures
The creatures are represented by a neural network. 
It contains the informaiton of the creature such as the position and the table.

## The Neural Network
The neural network is a feed forward neural network. By using weights and biases, the neural network can determine whether the action will happen or not.

## Genome
The genome is the DNA of the creature. It contains the information of connection weights and IDs of the source and destination neurons. The genome is used to create the neural network. The genome will be mutated to create a new generation of creatures. 

0-0011111-0-0000110-01111011010101110100010111000110
^SourceID ^DestID   ^Weight

### DNA
Collection of genomes. The DNA is used to create a creature. The DNA will be mutated and crossed over to create a new generation of creatures.

## Neurons
The neurons are the nodes of the neural network. The neurons are connected to each other. There are different types of neurons like
- Left Eye
- Right Eye
- Top Eye
- Bottom Eye
- Go Left
- Go Right
- Go Up
- Go Down
- Kill
- Inner Neuron

Input Neuron: The input neuron is the neuron that receives the input from the world. The input neuron is connected to the creature.

Output Neuron: The output neuron is the neuron that sends the output to the world and takes any action like moving the creature. The output neuron is connected to the creature.

## Creatures
The creatures are the agents of the simulation. The creatures are controlled by the neural network. The creatures are placed in the world. The creatures will live or die based on world conditions. 

## Population
The population will be randomly generated. Every individual in the population will also be randomly generated. 

## Activation Functions
- RelU: Rectified Linear Unit
- Sigmoid: Sigmoid Function

## Evolution
The creatures will be placed in the world. The creatures will be given a DNA. The DNA will be used to create the neural network. The neural network will be used to control the creature. The creature will live or die based on world conditions. The creatures who live will earn to reproduce. The creatures who die will not be able to reproduce. Also the creatures who live will pass on their DNA to the next generation. The DNA will be mutated and crossed over to create a new generation of creatures. 

## Table
It is 2D world simulation matrix which holds the creatures and sake of visaul representation of the world.

## Simulation
...