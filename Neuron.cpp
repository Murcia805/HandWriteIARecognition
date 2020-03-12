//
// Created by Guaberx on 11/4/2016.
//
#include "Neuron.h"

double sigmoidEquation(double x){
    return  1/(1+exp(-x));
}
double sigmoidDerivatedFunction(double x){
    return sigmoidEquation(x)*sigmoidEquation(1-x);
}


//Constructoras
Neuron::Neuron():transfered(1),transferedPrime(0),graphReference(NULL),inGraphIndex(0),sumOfProducts(0),
         miu(0.5),squaredError(0),squaredErrorDerivative(0){}
Neuron::~Neuron(){
//cout << "Neurona " << inGraphIndex << " Destruida" << endl;
}

//Sets de informacion sobre el entorno en el que se encuentra la neurona
void Neuron::setGraphReference(Graph<Neuron,double> *_graphReference){graphReference = _graphReference;}
void Neuron::setIndex(uint32_t i){inGraphIndex = i;}
void Neuron::setPredecessors(vector<uint32_t> i){predecessors = i;}
void Neuron::setSuccessors(vector<uint32_t> i){succesors = i;}

//Set
void Neuron::setOutputVal(double newData){
    transfered = newData; transferedPrime = newData*(1-newData);
}//transfered es el valor de salida de cada neurona

double Neuron::getTransfered()const{
    return transfered;
}//Retorna el output de la neurona
double Neuron::getTransferedPrime()const{
    return transferedPrime;
}//Retorna la derivada del valor de la neurona

void Neuron::SumProdToData(){
    //La sumatoria de los arcos por el valor de la neurona anterior se guarda en sumOfProducts
    assert(graphReference != NULL);
    assert(graphReference->getDataSize() > predecessors.back());
    sumOfProducts = 0;
    if(DEBUG)cout <<"Feeding forward neuron "<<inGraphIndex <<": "<<endl;
    for_each(predecessors.begin(),predecessors.end(),[this](uint32_t i){
        if(DEBUG)cout << "i = " << i << " \tdata of prev Neuron = " <<graphReference->infoVertice(i).transfered << " \tweight = " << graphReference->costoArco(i,inGraphIndex);
        sumOfProducts += graphReference->infoVertice(i).transfered * graphReference->costoArco(i,inGraphIndex);
        if(DEBUG)cout << " \tThe sumOfProd by the iteration = " <<sumOfProducts<<endl;
    });
    if(DEBUG){
        cout << "The Total sumOfProd = " << sumOfProducts
             << " \tThat value with the activation function = " << sigmoidEquation(sumOfProducts)
             << " \tIts derivative = " << sigmoidDerivatedFunction(sumOfProducts) << endl;
    }
}
void Neuron::transferFunction(double x){
    transfered = sigmoidEquation(x);
}//Funcion para normalizar el valor entre [0,1]/
/*
void Neuron::transferFunctionDerivative(double x){
    transferedPrime = sigmoidDerivatedFunction(x);
}//Funcion para normalizar el valor entre [0,1]*/
void Neuron::feedForward(){
    SumProdToData();
    transferFunction(sumOfProducts);
}//Realiza esas dos funciones



void Neuron::cforO(double target){
    //Calcula el error para cada conexion de la capa output
    weightsUpdate.clear();//Primero limpiamos el vector
    //(out - target)out(1-out)out
    double ActivacionDeNeuronaAnterior;
    squaredErrorDerivative = (transfered-target);
    transferedPrime = transfered*(1-transfered);
    for_each(predecessors.begin(),predecessors.end(),[this,&ActivacionDeNeuronaAnterior](uint32_t i){
        ActivacionDeNeuronaAnterior = graphReference->getData(i).getTransfered();
        weightsUpdate.push_back(squaredErrorDerivative*transferedPrime*ActivacionDeNeuronaAnterior);//Esto es lo que se va a actualizar ese arco por el error
    });
}
void Neuron::cforH(){
    //Calcula el error para cada conexion de la capa Oculta
    weightsUpdate.clear();//Primero limpiamos el vector
    //Primero sumamos para el error total de esta neurona
    double tempError = 0;
    double dErrorSiguiente;
    double dActivationSiguiente;
    double arcoSiguiente;
    //Primero Calculamos el error total de esta neurona
    for_each(succesors.begin(),succesors.end(),[this,&tempError,&dErrorSiguiente,&dActivationSiguiente,&arcoSiguiente](uint32_t i){
        dErrorSiguiente = graphReference->getData(i).squaredErrorDerivative;
        dActivationSiguiente = graphReference->getData(i).transferedPrime;
        arcoSiguiente = graphReference->costoArco(inGraphIndex,i);//El arco desde esta neurona a la siguiente
        tempError += dErrorSiguiente*dActivationSiguiente*arcoSiguiente;
    });
    transferedPrime = transfered*(1-transfered);//Derivamos la funcion de activacion de esta neurona
    double ActivacionDeNeuronaAnterior;
    for_each(predecessors.begin(),predecessors.end(),[this,&ActivacionDeNeuronaAnterior,&tempError](uint32_t i){
        ActivacionDeNeuronaAnterior = graphReference->getData(i).getTransfered();
        weightsUpdate.push_back(tempError*transferedPrime*ActivacionDeNeuronaAnterior);//Esto es lo que se va a actualizar ese arco por el error
    });
}

void Neuron::calculateSquaredError(double target){
    squaredError = (pow((target - transfered),2))/2;//transfered es el output
}
void Neuron::calculateSquaredErrorDerivative(double target){
    squaredErrorDerivative = -(target-transfered);//-(target - out)
}
void Neuron::calculatedOutdNet(){
    transferedPrime = transfered*(1-transfered);
}
void Neuron::calculatedNetdWi(){
    dNetdWi.clear();
    for_each(predecessors.begin(),predecessors.end(),[this](uint32_t i){
        dNetdWi.push_back(graphReference->getData(i).getTransfered());
    });
}
void Neuron::calculatedEtotaldWi(){
    weightsUpdate.clear();
    for_each(dNetdWi.begin(),dNetdWi.end(),[this](double i){
        //i es el valor de activacion de la neurona anterior
        weightsUpdate.push_back(squaredErrorDerivative*transferedPrime*i);//TODO COMO QUE NO SE GUARDA PORQUE ES MIERDA
    });
}
double Neuron::getSquaredError()const{
    return squaredError;
}
double Neuron::getSquaredErrorDerivative()const{
    return squaredErrorDerivative;
}
double Neuron::getdOutdNet()const{
    return transferedPrime;
}
vector<double> Neuron::getdNetdWi(){
    return dNetdWi;
}
vector<double> Neuron::getdEtotaltdWi(){
    return weightsUpdate;
}

void Neuron::calculateSquaredErrorDerivativeH(){
    //Es la suma de los errores de las neuronas siguientes
    /*
     * dE0i/dNeti = squaredErrorDerivative[i]*transferedDerivative[i]
     * SUM     (dNeti/dOuti = arco(self,i)*dE0i/dNeti)
     */
    squaredErrorDerivative = 0;
    for_each(succesors.begin(),succesors.end(),[this](uint32_t i){
        squaredErrorDerivative += graphReference->getArco(inGraphIndex,i)*(graphReference->getData(i).getSquaredErrorDerivative()*graphReference->getData(i).getdOutdNet());
    });
}

void Neuron::printWeightsUpdates(){
    cout << "--------Neuron " << inGraphIndex << "--------"<<endl;
    uint32_t j = 0;
    assert(predecessors.size() == weightsUpdate.size());
    for_each(weightsUpdate.begin(),weightsUpdate.end(),[this,&j](double i){cout << "Arco: " <<predecessors.at(j++)<<","<<inGraphIndex<<" -> "<< i <<endl;});
}
void Neuron::printPredeccessors(){
    cout << "Neuron "<<inGraphIndex<<" Predecesores"<<endl;
    for_each(predecessors.begin(),predecessors.end(),[](uint32_t i){cout << i << ", ";});
    cout << endl;
}
void Neuron::printSuccessors(){
    cout << "Neuron "<<inGraphIndex<<" Sucesores" << endl;
    for_each(succesors.begin(),succesors.end(),[](uint32_t i){cout << i << ", ";});
    cout << endl;
}

uint32_t Neuron::getIndex()const{return inGraphIndex;}//Retorna el index de esta neurona en el grafo

void Neuron::updateWeights(){
    uint32_t j = 0;
    double temp;
    for_each(predecessors.begin(),predecessors.end(),[this,&temp,&j](uint32_t i){
        temp = graphReference->costoArco(i,inGraphIndex) - miu*weightsUpdate.at(j++);//El valor del arco menos el error
        graphReference->setArco(i,inGraphIndex,temp);
    });
}
