#include <stdlib.h>

typedef struct ct {
	double * data;
	size_t nx;
	size_t ny;
	size_t nz;
} CT;

void renderVisualization(char * filename, unsigned int * volumeDimensions);