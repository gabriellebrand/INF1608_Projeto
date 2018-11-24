#include <stdlib.h>
#include <stdio.h>
#include "FileManager.h"
#include "VolumeRendering.h"


int main () {
	double test[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 1.0};
	unsigned int dimensions[3] = {256, 256, 99};
	char path[] = "vridados-head-8bit.raw";

	renderVisualization(path, dimensions);

	return 0;
}