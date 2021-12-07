#ifndef _UTILS_H_
#define _UTILS_H_
/*
 * utils.h (c) Shish 2005
 * 
 * functions for use within mesh generators
 */
#include "../shinymath.h"

#ifdef __WIN32
  #ifdef BUILD_DLL
    // the dll exports
    #define EXPORT __declspec(dllexport)
  #else
    // the exe imports
    #define EXPORT __declspec(dllimport)
  #endif
#else
  #define EXPORT
#endif


// utilities
#define addPoint2d(x, y)    addPoint(x, y, 0, 0.2, 0.2, 0.2, 1);
#define addPoint3d(x, y, z) addPoint(x, y, z, 0.2, 0.2, 0.2, 1);
#define addBreakPoint()     addPoint(0, 0, 0,   0,   0,   0, 0);
void addPoint(float x, float y, float z, float r, float g, float b, int draw);
float rad(int deg);

// functions for the module to supply
int generate();

// functions for the parent to call
mesh_t *getMesh(int time);

extern int time;

#endif
