#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct ct {
	unsigned char * data;
	size_t nx;
	size_t ny;
	size_t nz;
} CT;

//estrutura para guardar os valores do CT scan
static CT scan;

unsigned char* readScanFile (char * path, size_t fileSize) {
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

void writePGMFile(double * values, int width, int height) {
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

//raio é uma reta parametrica definida por um vetor de origem e um vetor direção
// (ox, oy, oz) + t*(dx, dy, dz)
//retorna o indice do volume dado um t
int ray(int* origin, int* direction, double t) {
	int i, j, k;
	i = origin[0] + t*direction[0];
	j = origin[1] + t*direction[1];
	k = origin[2] + t*direction[2];

	return k*scan.nx*scan.ny + t*scan.nx + i;
}

//@TODO
//será necessário adaptar o simpson para calcular o índice a partir da função ray
double simpson (double (*f) (double), double a, double b, double h) {
	return 0.0;
}

//recebe o indice calculado pela função do raio
double density(int r) {
	return (double) scan.data[r];
}

//funcao de transferência
double transferFunction(double v) {
	if (v < 0.3)
		return 0.0;
	else
		return 0.05*(v - 0.3);
}

//τ(d(t))
double tauDt(double t) {
	double d = density(t);
	return transferFunction(d);
}

//calcula integral do expoente da função de renderização
double exponentFunction(double s, double h) {
	return simpson(&tauDt, 0.0, s, h);
}

//Função de renderização volumétrica
double volumeRender(s, h) {
	double d = density(s);

	return tauDt(d)*exp(-(exponentFunction(s, h)));
}


int main () {
	double test[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 1.0};

	scan.nx = 256;
	scan.ny = 256;
	scan.nz = 99;
	scan.data = readScanFile("vridados-head-8bit.raw", scan.nx*scan.ny*scan.nz);

	// for (int i = 0; i < fileSize; i++) {
	// 	printf("%d \n", scanData[i]);
	// }

	writePGMFile(test, 4, 2);

	return 0;
}