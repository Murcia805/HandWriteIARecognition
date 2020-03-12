//
// Created by Guaberx on 11/4/2016.
//

#ifndef HANDWRITERECOGNITION_TOPOLOGY_H
#define HANDWRITERECOGNITION_TOPOLOGY_H

#include <iostream>
#include <vector>
using std::vector;
//(cuantas neuronas tiene la primera capa, cuantas neuronas tiene la capa oculta, cuantas neuronas tiene la capa de salida)
class Topology{
public:
    Topology(uint32_t _nInputLayers, uint32_t _nHiddenLayers, uint32_t _nResultLayers);

    uint32_t getNInputLayers()const;
    uint32_t getNHiddenLayers()const;
    uint32_t getNResultLayers()const;
private:
    uint32_t nInputLayers;
    uint32_t nHiddenLayers;
    uint32_t nResultLayers;
};//Contiene la informacion para crear la red neuronal



#endif //HANDWRITERECOGNITION_TOPOLOGY_H
