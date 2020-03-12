//
// Created by Guaberx on 11/4/2016.
//

#include "Topology.h"

Topology::Topology(uint32_t _nInputLayers, uint32_t _nHiddenLayers, uint32_t _nResultLayers)
    :nInputLayers(_nInputLayers),nHiddenLayers(_nHiddenLayers),nResultLayers(_nResultLayers){}

uint32_t Topology::getNInputLayers()const{ return nInputLayers;}
uint32_t Topology::getNHiddenLayers()const{ return nHiddenLayers;}
uint32_t Topology::getNResultLayers()const{ return nResultLayers;}