#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FileManager.h"

unsigned char* FM_readScanFile (char * path, size_t fileSize) {
	FILE *fp;
	unsigned char *buffer;

	buffer = (unsigned char *)malloc(sizeof(char)*fileSize);

	fp = fopen(path, "r");

	if (fp) {
		if (fread(buffer, sizeof(char), fileSize, fp) == fileSize) {
			return buffer;
		}
	}

	printf("Ocorreu um erro na leitura do arquivo.");
		return NULL;
}

void FM_writePGMFile(double * values, int width, int height) {
	FILE *vriOutput;
	int maxColorValue = 255, i, k;

	vriOutput = fopen( "vriOutput.pgm" , "w");

	if (vriOutput) {
		//cabeçalho
		fprintf(vriOutput, "P2\n%d %d\n%d\n", width, height, maxColorValue); 

		//valores
		for (i = 0; i < height; i++) {
			for (k = 0; k < width; k++) {
				fprintf(vriOutput, "%d ", (unsigned char)(255*values[i*width + k]));
			}
			fprintf(vriOutput, "\n");
		}

		fclose(vriOutput);
	}
}