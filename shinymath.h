#ifndef _SHINYMATH_H_
#define _SHINYMATH_H_
/*
 * shinymath.h (c) Shish 2005
 * 
 * things common to the viewer and generators
 */

typedef struct {
	float x, y, z;
	float r, g, b;
    int draw;
} point3d;

typedef struct {
	point3d *vertices;
	int pcount;
} mesh_t;

#endif

