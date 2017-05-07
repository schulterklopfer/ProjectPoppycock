//
//  globals.cpp
//  PoppyCock
//
//  Created by jash on 07.05.17.
//
//

#include "Globals.h"
#include "ofUtils.h"

float Globals::sTimeScale = 1.0f;
__int64_t Globals::sElapsedTimeMillis = 0;
__int64_t Globals::sLastTimeMillis = 0;

__int64_t Globals::getElapsedTimeMillis() {
    return sElapsedTimeMillis;
}

void Globals::resetElapsedTimeMillis() {
    sElapsedTimeMillis = 0;
}

float Globals::getTimeScale() {
    return Globals::sTimeScale;
}

void Globals::setTimeScale( const float ts ) {
    Globals::sTimeScale = ts;
}


void Globals::update() {
    const __int64_t elapsed = ofGetElapsedTimeMillis();
    const int delta = elapsed - sLastTimeMillis;
    sLastTimeMillis = elapsed;
    sElapsedTimeMillis += (int)((float)delta*sTimeScale);
}
