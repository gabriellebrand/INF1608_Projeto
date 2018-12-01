#include "Ray.h"
#include <stdlib.h>
#include <stdio.h>

static unsigned int _nx;
static unsigned int _ny;
static unsigned int _nz;
static int _origin[3];
static int _direction[3];

void Ray_setDimensions(unsigned int nx, unsigned int ny, unsigned int nz) {
	_nx = nx;
	_ny = ny;
	_nz = nz;
}

void Ray_setOrigin(int ox, int oy, int oz) {
	_origin[0] = ox;;
	_origin[1] = oy;;
	_origin[2] = oz;;
}

void Ray_setDirection(int dx, int dy, int dz) {
	_direction[0] = dx;
	_direction[1] = dy;
	_direction[2] = dz;
}

// raio é uma reta parametrica definida por um vetor de origem e um vetor direção
// (ox, oy, oz) + t*(dx, dy, dz)
// retorna o indice do volume dado um t
int Ray_trace(int t) {
	int i, j, k;
	i = _origin[0] + t*_direction[0];
	j = _origin[1] + t*_direction[1];
	k = _origin[2] + t*_direction[2];

	return k*_nx*_ny + j*_nx + i;
}