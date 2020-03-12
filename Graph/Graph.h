//
// Created by Guaberx on 10/21/2016.
//

#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H
#define CLOSED (~0)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::for_each;
using std::ofstream;
using std::ifstream;
using std::ios;
template <typename T, typename G>
class Graph{
public:
    T &getData(uint32_t i){return data.at(i);}
    G &getArco(uint32_t i, uint32_t j){return matrix.at(i).at(j);}
    Graph(){}
    Graph(uint32_t numberOfElements){
        for (int k = 0; k < numberOfElements; ++k) {
            data.push_back(T());
        }
        for (int i = 0; i < numberOfElements; ++i) {
            matrix.push_back(vector<G>());//La matriz es cuadrada y no hay conexiones
            for (int j = 0; j < numberOfElements; ++j) {
                matrix.at(i).push_back(G(CLOSED));
            }
        }
    }
    Graph(const vector<T> newData):data(newData){
        for (int i = 0; i < newData.size(); ++i) {
            matrix.push_back(vector<G>());//La matriz es cuadrada y no hay conexiones
            for (int j = 0; j < newData.size(); ++j) {
                matrix.at(i).push_back(G(CLOSED));
            }
        }
    }
    Graph(const vector<T> newData,uint32_t extraLayers):data(newData){
        //Acomodamos la matriz
        for (int i = 0; i < newData.size(); ++i) {
            matrix.push_back(vector<G>());//La matriz es cuadrada y no hay conexiones
            for (int j = 0; j < newData.size(); ++j) {
                matrix.at(i).push_back(G(CLOSED));
            }
        }
        for (int k = 0; k < extraLayers; ++k) {
            //Se agrega uno
            matrix.push_back(vector<G>());
            //El que se agrego toca igualarlo en tamano a las otras filas
            for (int j = 0; j < matrix.at(0).size(); ++j) {//TODO REVISAR CUANTAS
                matrix.at(data.size()+k).push_back(G(CLOSED));//TODO VOLVER ALEATORIO
            }
            //Luego se le agrega una columna a cada fila para que quede cuadrada
            for (int i = 0; i < matrix.size(); ++i) {//TODO REVISAR CUANTAS
                matrix.at(i).push_back(G(CLOSED));//TODO VOLVER ALEATORIO
            }
        }
        //Acomodamos data
        for (int l = 0; l < extraLayers; ++l) {
            data.push_back(T());
        }
    }

    void addElements(T ElemType,uint32_t numberOfElements){
        for (int k = 0; k < numberOfElements; ++k) {
            data.push_back(ElemType);
        }
        for (int i = 0; i < numberOfElements; ++i) {
            matrix.push_back(vector<G>());//La matriz es cuadrada y no hay conexiones
            //Igualamos el nuevo vector
            for (int k = 0; k < matrix.size() - 1; ++k) {//-1 porque se acabo de anadir uno y no lo queremos contar
                matrix.at(matrix.size()-1).push_back(G(CLOSED));
            }
            //Le agregamos uno a cada uno de los anteriores
            for (int j = 0; j < matrix.size(); ++j) {
                matrix.at(j).push_back(G(CLOSED));
            }
        }
    }

    uint32_t getDataSize()const{return data.size();}
    uint32_t getRowSize()const{return matrix.size();}
    uint32_t getColSize()const{return matrix.empty()? 0:matrix.at(0).size();}

    void insVertix(T newElem);//Agrega un elemento a data y en la matriz se inicializa sin conexiones
    void setVertix(uint32_t i,T newElem);//se describe sola
    void setArco(uint32_t i,uint32_t j,G newArco);//i,j de matrix
    void elimArco(uint32_t i,uint32_t j);//Lo cierra

    T infoVertice(uint32_t i) const;
    G costoArco(uint32_t i,uint32_t j) const;

    vector<uint32_t> predecessors(uint32_t i)const;//retorna los predecessors del vértice vi
    vector<uint32_t> successors(uint32_t i)const;//retorna la lista de successors del vertice vi

    //Persistencia
    void clean();

    void save(string filePath)const;
    void load(string filePath);

    void printData()const{for_each(data.begin(),data.end(),[](T i){
            cout << "<" << i << ">";});
        cout << endl;
    }
    void printMatrix()const{for_each(matrix.begin(),matrix.end(),[](vector<G> i){
            for_each(i.begin(),i.end(),[](G j){
                cout << j << " ";
            });
            cout << endl;
        });
    }
    //Destructora
    ~Graph(){
        clean();
    }//Devuelve al sistema toda la memoria dinamica utilizada
private:
    vector<T> data;
    vector<vector<G>> matrix;
};

template <typename T, typename G>
void Graph<T,G>::insVertix(T newElem){
    data.push_back(newElem);
    vector<G> tempVector;
    for (int i = 0; i < matrix.size(); ++i) {
        tempVector.push_back(CLOSED);
    }
    matrix.push_back(tempVector);
    for (int j = 0; j < matrix.size(); ++j) {
        matrix.at(j).push_back(CLOSED);
    }
};

template <typename T, typename G>
void Graph<T,G>::setVertix(uint32_t i,T newElem){data.at(i) = newElem;};

template <typename T, typename G>
void Graph<T,G>::setArco(uint32_t i,uint32_t j,G newArco){matrix.at(i).at(j) = newArco;};

template <typename T, typename G>
void Graph<T,G>::elimArco(uint32_t i,uint32_t j){ matrix.at(i).at(j) = CLOSED;};

template <typename T, typename G>
T Graph<T,G>::infoVertice(uint32_t i) const{
    return data.at(i);
};

template <typename T, typename G>
G Graph<T,G>::costoArco(uint32_t i,uint32_t j) const{ return matrix.at(i).at(j);};

template <typename T, typename G>
vector<uint32_t> Graph<T,G>::predecessors(uint32_t i)const{
    vector<uint32_t> result;
    for (uint32_t j = 0; j < matrix.size(); ++j) {
        if(matrix.at(j).at(i) != CLOSED){
            result.push_back(j);
        }
    }
    return result;
};

template <typename T, typename G>
vector<uint32_t> Graph<T,G>::successors(uint32_t i)const{
    vector<uint32_t> result;
    for (uint32_t j = 0; j < matrix.size(); ++j) {
        if(matrix.at(i).at(j) != CLOSED) {
            result.push_back(j);
        }
    }
    return result;
};

//Persistencia
template <typename T, typename G>
void Graph<T,G>::clean(){data.clear(); for_each(matrix.begin(),matrix.end(),[](vector<G> i){ i.clear();}); matrix.clear();};

template <typename T, typename G>
void Graph<T,G>::save(string filePath)const{
    ofstream file(filePath, ios::out | ofstream::binary);
    //Header Of Data Size
    uint32_t dataSize = data.size();
    file.write((char*)&dataSize,sizeof(uint32_t));
    //Header Of Matrix Size
    uint32_t matrixSize = matrix.size();
    file.write((char*)&matrixSize,sizeof(uint32_t));
    //Writes the data
    for (int i = 0; i < data.size(); ++i) {
        file.write((char*)&data[i],sizeof(T));
    }
    //Writes the matrix
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix.at(i).size(); ++j) {
            file.write((char*)&matrix.at(i).at(j),sizeof(G));
        }
    }
}

template <typename T, typename G>
void Graph<T,G>::load(string filePath){
    clean();
    ifstream file(filePath, ios::in | ifstream::binary);
    uint32_t dataSize = data.size();
    uint32_t matrixSize = matrix.size();
    file.read((char*)&dataSize,sizeof(uint32_t));
    file.read((char*)&matrixSize,sizeof(uint32_t));

    T tempDataElem;
    for (int i = 0; i < dataSize; ++i) {
        file.read((char*)&tempDataElem,sizeof(T));
        data.push_back(tempDataElem);
    }
    G tempMatrixElem;
    vector<G> tempVector;
    for (int j = 0; j < matrixSize; ++j) {
        matrix.push_back(tempVector);
        for (int i = 0; i < matrixSize; ++i) {
            file.read((char*)&tempMatrixElem,sizeof(G));
            matrix.at(j).push_back(tempMatrixElem);
        }
    }
}


#endif //GRAPH_GRAPH_H
