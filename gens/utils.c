/*
 * utils.h (c) Shish 2005
 * 
 * functions for use within mesh generators
 */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

static mesh_t *mesh;
static int palloc = 0;
int time = 0;

/*
 * add a point to the vertex chain
 */
void addPoint(float x, float y, float z, float r, float g, float b, int draw) {
	mesh->vertices[mesh->pcount].x = x;
	mesh->vertices[mesh->pcount].y = y;
	mesh->vertices[mesh->pcount].z = z;
	mesh->vertices[mesh->pcount].r = r;
	mesh->vertices[mesh->pcount].g = g;
	mesh->vertices[mesh->pcount].b = b;
	mesh->vertices[mesh->pcount].draw = draw;

	mesh->pcount++;
	if(mesh->pcount == palloc) {
		palloc += 20;
		mesh->vertices = (point3d*)realloc(mesh->vertices, sizeof(point3d)*palloc);
	}
	if(mesh->pcount < palloc-20) {
		palloc -= 20;
		mesh->vertices = (point3d*)realloc(mesh->vertices, sizeof(point3d)*palloc);
	}
}


/*
 * convert degrees to radians, useful for functions of time as
 * sin(small float) gives saner results than sin(large int)
 */
float rad(int deg) {
	return (float)deg/360*2*M_PI;
}


/*
 * call the generate function and return the result
 */
EXPORT mesh_t *getMesh(int t) {
	mesh = (mesh_t*)malloc(sizeof(mesh_t));
	palloc = 20;
	mesh->vertices = (point3d*)malloc(sizeof(point3d)*palloc);
	mesh->pcount = 0;

	time = t;
	generate();

	return mesh;
}


int main(int argc, char *argv[]) {
	int i=0;

	mesh = (mesh_t*)malloc(sizeof(mesh_t));
	palloc = 20;
	mesh->vertices = (point3d*)malloc(sizeof(point3d)*palloc);
	mesh->pcount = 0;

	generate();

	for(i=0; i<mesh->pcount; i++) {
		printf("%f %f %f\n", mesh->vertices[i].x, mesh->vertices[i].y, mesh->vertices[i].z);
	}

	free(mesh->vertices);
	free(mesh);
}
