//
//  globals.hpp
//  PoppyCock
//
//  Created by jash on 07.05.17.
//
//

#ifndef Globals_h
#define Globals_h

#include <stdio.h>

class Globals {

private:
    static __int64_t sLastTimeMillis;
    static __int64_t sElapsedTimeMillis;
    static float sElapsedTimef;
    static float sTimeScale;
    
public:

    static float getTimeScale();
    static void setTimeScale( const float ts );
    static __int64_t getElapsedTimeMillis();
    static float getElapsedTimef();
    static void resetElapsedTime();
    static void update();
};



#endif /* Globals_h */
