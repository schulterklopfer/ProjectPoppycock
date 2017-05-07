//
//  GPUEffect.cpp
//  PoppyCock
//
//  Created by jash on 07.05.17.
//
//

#include "GPUEffect.h"

int GPUEffect::getTypeFlags() {
    return Interactive::Type::GPU_EFFECT|Interactive::Type::EFFECT|Interactive::Type::ENTITY;
}
