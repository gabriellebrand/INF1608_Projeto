#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "FileManager.h"

typedef struct ct {
	unsigned char * data;
	size_t nx;
	size_t ny;
	size_t nz;
} CT;

//estrutura para guardar os valores do CT scan
static CT scan;

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
//retorna a densidade na posicao r do volume do CT scan
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
	scan.data = FM_readScanFile("vridados-head-8bit.raw", scan.nx*scan.ny*scan.nz);

	// for (int i = 0; i < fileSize; i++) {
	// 	printf("%d \n", scanData[i]);
	// }

	FM_writePGMFile(test, 4, 2);

	return 0;
}