//
//  Utility.h
//  SnowWalker
//
//  Created by 小川 穣 on 2014/02/25.
//
//

#ifndef SnowWalker_Utility_h
#define SnowWalker_Utility_h

inline int utRand(int range) {
    return (rand() % (range * 100)) / 100;
}

inline int utRand(int rangeMin, int rangeMax) {
    return rangeMin + (rand() % ((rangeMax - rangeMin) * 100)) / 100;
}

inline float utRand(float range) {
    return range * rand() / RAND_MAX;
}

inline float utRand(float rangeMin, float rangeMax) {
    return rangeMin + (rangeMax - rangeMin) * rand() / RAND_MAX;
}

#endif
