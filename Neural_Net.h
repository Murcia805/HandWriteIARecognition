//
// Created by Guaberx on 10/24/2016.
//

#ifndef HANDWRITERECOGNITION_NEURAL_NET_CLEANUP_H
#define HANDWRITERECOGNITION_NEURAL_NET_CLEANUP_H
#include <vector>
#include <cmath>
#include <random>
#include <cassert>
#include "Neuron.h"
#include "Bias_Neuron.h"
#include "Graph/Graph.h"
#include "Topology.h"

using std::exp;
using std::rand;
using std::sqrt;

class Net{
public:
    //La data del graph cambia, los pesos varian en el backpropagation, en el forward es la data
    //ResultLayers contiene lo del resultado.
    Net(Topology topology);

    void train(const vector<vector<double>> &inputVals, const vector<vector<double>> &wantedVal, uint32_t times);//TODO INVENTATELA. UTILIZA backPropagation y feedForward
    vector<double> getResult(vector<double> &inputVals);//TODO Simplemente es feedForward y que retorne la capa de salida de la red

private:
    void setInputLayer(const vector<double> &inputVals);
    void feedForward(const vector<double> &inputVals);//Activa las neuronas y cada neurona sala su sumatoria de las anteriores por sus arcos
    void calculateError(vector<double>&targetVals);//TODO HACIENDO ESTA!!!!
    void calculateErrorDerivative(vector<double>&targetVals);//TODO HACIENDO ESTA!!!! No se usa porque calculamos la derivada en la funcion anterior
    void updateWeights();
    void printAllWeightsUpdates();
    void printAllPredecessors();
    void printAllSuccessors();
    double getError(){return totalSquaredError;}
    void backPropagation(const vector<double> &targetVals);//Calculate the error for each output neuron //TODO AQUI SE LLAMA LA FUNCION algo() DE CADA NEURONA
    void printNeurons();//Imprime el vector del grafo
    void printConnections();//Imprime la matriz del grafo

private:
    double totalSquaredError;
    vector<double> lastResult;

    Graph<Neuron,double> connections;
    uint32_t nBiasLayers;
    uint32_t nInputLayers;//Numero de neuronas en la primera capa
    uint32_t nHiddenLayers;//Numero de neuronas en la capa oculta
    uint32_t nResultLayers;//Numero de neuronas de salida
    uint32_t totalNeurons;//Numero Total de neuronas
};

#endif //HANDWRITERECOGNITION_NEURAL_NET_CLEANUP_H
