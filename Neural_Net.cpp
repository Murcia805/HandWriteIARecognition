//
// Created by Guaberx on 11/4/2016.
//
#include "Neural_Net.h"
#define DEBUG2 0
double RandomDouble(double a, double b) {
    //Retorna un valor aleatorio de tipo double entre a y b
    double random = ((double) rand()) / (double) RAND_MAX;
    double diff = b - a;
    double r = random * diff;
    return a + r;
}
Net::Net(Topology topology)
        :totalSquaredError(0),
         nInputLayers(topology.getNInputLayers()),
         nHiddenLayers(topology.getNHiddenLayers()),
         nResultLayers(topology.getNResultLayers()),
         nBiasLayers(2),
         connections(Graph<Neuron, double>())

{
    totalNeurons = 2 + topology.getNInputLayers()+topology.getNHiddenLayers()+topology.getNResultLayers();
    //Create the Graph
    connections.addElements(Neuron(),totalNeurons);//we add the bias 1 input, 1 hidden

    double temp;
    //Set the Graph reference of the neurones
    for (int i = 0 ; i < connections.getDataSize(); ++i) {
        connections.getData(i).setGraphReference(&connections);
    }
    //Set Connections from input and bias layers to hiddelLayer
    for (int j = 1+1+nInputLayers; j < 1+1+nInputLayers+nHiddenLayers; ++j) {
        for (int i = 0; i < 1+nInputLayers; ++i) {//Comienza desde 0 para tomar el bias
            connections.setArco(i,j,RandomDouble(-100,100));
        }
    }
    //Set Connections from Hidden layer to Result Layer
    for (int j = 1+1+nInputLayers+nHiddenLayers; j < totalNeurons; ++j) {
        for (int i = 1+nInputLayers; i < 1+1+nInputLayers+nHiddenLayers; ++i) {
            connections.setArco(i,j,RandomDouble(-100,100));
        }
    }
    //Predecesores, succesores y el index en el grafo de cada neurona
    vector<uint32_t> tempPredecesores;
    vector<uint32_t> tempSucesores;
    //El index de la neurona en el grafo
    //predecessors de cada neurona
    for (int k = 0; k < totalNeurons; ++k) {
        connections.getData(k).setIndex(k);
        tempPredecesores = connections.predecessors(k);
        tempSucesores = connections.successors(k);
        connections.getData(k).setPredecessors(tempPredecesores);
        connections.getData(k).setSuccessors(tempSucesores);
    }
}

void Net::setInputLayer(const vector<double> &inputVals){
    //Asserts
    assert(nInputLayers == inputVals.size());
    if(DEBUG){
        cout << "EL Vector Input = { ";
        for_each(inputVals.begin(),inputVals.end(),[](double i){ cout << i << ", ";});cout << "}"<< endl;
    }
    //Set the first layer to the inputs
    uint32_t tempIndex = 1;
    for_each(inputVals.begin(),inputVals.end(),[this,&tempIndex](double i){connections.getData(tempIndex++).setOutputVal(i);});
}

void Net::train(const vector<vector<double>> &inputVals, const vector<vector<double>> &wantedVal, uint32_t times) {
    assert(inputVals.size() == wantedVal.size());//Cada uno debe tener un respectivo resultado
    uint32_t j = 0;
    uint32_t loop = 0;
    vector<double> tempWantedVal;
    double temp;
    for (int i = 0; i < times; ++i) {
        j = 0;
        loop = 1;
        if(DEBUG2)cout << "Epoch: "<< i+1 << endl;
        for_each(inputVals.begin(),inputVals.end(),[this,&j,&wantedVal,&tempWantedVal,&temp,&loop](vector<double> iv){
            lastResult.clear();
            feedForward(iv);
            tempWantedVal = wantedVal.at(j++);
            calculateError(tempWantedVal);
            updateWeights();
            //Copiamos el vector resultado en last Result
            for (int k = 0; k < nResultLayers; ++k) {
                temp = connections.getData(2+nInputLayers+nHiddenLayers+k).getTransfered();
                lastResult.push_back(temp);
            }
            if(DEBUG2){
                cout << loop++ << ": ";
                for_each(tempWantedVal.begin(),tempWantedVal.end(),[](double ev){
                    cout << "<" << ev << ">";
                });cout << " vs ";
                for_each(lastResult.begin(),lastResult.end(),[](double ls){
                    cout << "<" << ls << ">";
                });cout << "  S_Error = " << totalSquaredError <<endl;
                //printAllWeightsUpdates();
            }
        });
    }
}

void Net::feedForward(const vector<double> &inputVals) {
    setInputLayer(inputVals);
    //Feed forwads the other layers
    for (int i = 2+nInputLayers; i < totalNeurons; ++i) {//Exeptuamos la bias de la hidden layer
        connections.getData(i).feedForward();
        if(DEBUG)cout << "Data of the " << i <<"th neuron = " << connections.getData(i).getTransfered() <<endl;// << endl;
    }
    //Despues de esto cada neurona esta activada
}

void Net::calculateError(vector<double>&targetVals){
    totalSquaredError = 0;//reseteamos cada vez para ver el nuevo error
    double target;
    //Primero la capa output
    /*for (int i = 0; i < nResultLayers; ++i) {
        target = targetVals.at(i);
        //2+nInputLayers+nHiddenLayers porque vamos a evaluar la ultima capa
        connections.getData(i+2+nInputLayers+nHiddenLayers).calculateSquaredError(target);//El error
        connections.getData(i+2+nInputLayers+nHiddenLayers).calculateSquaredErrorDerivative(target);//Derivada del error
        connections.getData(i+2+nInputLayers+nHiddenLayers).calculatedOutdNet();
        connections.getData(i+2+nInputLayers+nHiddenLayers).calculatedNetdWi();
        connections.getData(i+2+nInputLayers+nHiddenLayers).calculatedEtotaldWi();
        totalSquaredError += connections.getData(i+2+nInputLayers+nHiddenLayers).getSquaredError();
    }
    //Luego la capa hidden
    for (int j = 0; j < nHiddenLayers + 1; ++j) {//+1 para que la neurona bias actualice tambien
        connections.getData(j+2+nInputLayers).calculateSquaredErrorDerivativeH();
        connections.getData(j+2+nInputLayers).calculatedOutdNet();
        connections.getData(j+2+nInputLayers).calculatedNetdWi();//En esta el cambio es diferente por cada predecesor.
        connections.getData(j+2+nInputLayers).calculatedEtotaldWi();
    }*/
    //Primero la capa output
    for (int i = 0; i < nResultLayers; ++i) {
        target = targetVals.at(i);
        //2+nInputLayers+nHiddenLayers porque vamos a evaluar la ultima capa
        connections.getData(i+2+nInputLayers+nHiddenLayers).calculateSquaredError(target);//El error
        totalSquaredError += connections.getData(i+2+nInputLayers+nHiddenLayers).getSquaredError();
        connections.getData(i+2+nInputLayers+nHiddenLayers).cforO(target);
    }
    //Luego la capa hidden
    for (int j = 0; j < nHiddenLayers + 1; ++j) {//+1 para que la neurona bias actualice tambien
        connections.getData(j+2+nInputLayers).cforH();
    }
    if(DEBUG)cout << "Total Squared Error = " << totalSquaredError << endl;
}

void Net::updateWeights(){
    for (int i = totalNeurons-1; i > nInputLayers+1; --i) {
        connections.getData(i).updateWeights();
    }
}

void Net::printAllWeightsUpdates(){
    for (int i = 0; i < connections.getDataSize(); ++i) {
        connections.getData(i).printWeightsUpdates();
    }
}
void Net::printAllPredecessors(){
    for (int i = 0; i < connections.getDataSize(); ++i) {
        connections.getData(i).printPredeccessors();
    }
}
void Net::printAllSuccessors(){
    for (int i = 0; i < connections.getDataSize(); ++i) {
        connections.getData(i).printSuccessors();
    }
}

void Net::backPropagation(const vector<double> &targetVals) {
    assert(nResultLayers == targetVals.size());

}

vector<double> Net::getResult(vector<double> &inputVals) {
    feedForward(inputVals);
    vector<double> result;
    cout << "Result:\t";
    for (int i = 2+nInputLayers+nHiddenLayers; i < totalNeurons; ++i) {
        cout << "<" << connections.getData(i).getTransfered() <<">";
        result.push_back(connections.getData(i).getTransfered());
    }cout << "\t\tMSE:\t"<< totalSquaredError << endl;
    return move(result);
}

void Net::printNeurons(){
    for (int i = 0; i < connections.getDataSize(); ++i) {
        cout << "<" << connections.getData(i).getTransfered() << ">";
    }cout << endl;}//Imprime el vector del grafo
void Net::printConnections(){connections.printMatrix();}//Imprime la matriz del grafo

