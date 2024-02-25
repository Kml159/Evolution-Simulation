# Evolution of Creatures - Simulation

## How to run?  
You can run the main.cpp  
  
Linux & Unix:  
```bash
cd "path/to/folder/Evolution-Simulation/" && g++ -std=c++17 main.cpp -o main && "path/to/folder/Evolution-Simulation/"main
```

## Introduction
This project is a simulation of the evolution of a population of creatures. The creatures are represented by a neural network. The neural network is trained using a genetic algorithm. The creatures are controlled by the neural network. The creatures are placed in a 2D world. 

## The World
The world is a 2D grid. It contains creatures

## Creatures
The creatures are represented by a neural network. 
It contains the informaiton of the creature such as the position and the table.

## Neural Network
The neural network is a feed forward neural network. By using weights and biases, the neural network can determine whether the action will happen or not.

## Genome
The genome is the DNA of the creature. It contains the information of connections between neurons.  

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
- Inner Neuron
- Kill (Incomplete)

Input Neuron: The input neuron is the neuron that receives the input from the world. The input neuron is connected to the creature.
Output Neuron: The output neuron is the neuron that sends the output to the world and takes any action like moving the creature. The output neuron is connected to the creature.

## Evolution
The creatures will be placed in the world. The creatures will be given a random DNA. The DNA will be used to create the neural network. The neural network will be used to control the creature. The creature will live or die based on world conditions, then creatures who live will earn to reproduce. Reproduces will genenerate next generation with cross-over and mutation.

### Note
Reprodution is problematic. Crossover is happening with irrelevant connections, thus causing loss of genetic information of the reproducers. This prevents the individual's DNA to converge!

## Simulation
![Alt text](img/sim01.png)
![Alt text](img/sim02.png)
![Alt text](img/sim03.png)

