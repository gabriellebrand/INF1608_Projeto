#include <stdlib.h>
#include <stdio.h>
#include "FileManager.h"


int main () {
	double test[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 1.0};

	FM_writePGMFile(test, 4, 2);

	return 0;
}