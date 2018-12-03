#include <stdlib.h>
#include <stdio.h>
#include "FileManager.h"
#include "VolumeRendering.h"


int main () {
	unsigned int dimensions[3] = {256, 256, 99};
	char path[] = "vridados-head-8bit.raw";

	VR_renderVisualization(path, dimensions);

	return 0;
}