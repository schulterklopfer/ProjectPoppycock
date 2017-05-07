//
//  ShaderEffect.cpp
//  PoppyCock
//
//  Created by jash on 07.05.17.
//
//

#include "ShaderEffect.h"

int ShaderEffect::getTypeFlags() {
    return Interactive::Type::SHADER_EFFECT|Interactive::Type::EFFECT|Interactive::Type::ENTITY;
}
