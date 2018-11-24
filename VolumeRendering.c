#include "VolumeRendering.h"
#include "Ray.h"
#include "FileManager.h"
#include "Simpson.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//estrutura para guardar os valores do CT scan
static CT _scan;

//passo de integração
static double _h = 4;

//funcao de transferência
double transferFunction(double v) {
	if (v < 0.3)
		return 0.0;
	else
		return 0.05*(v - 0.3);
}

//recebe o indice calculado pela função do raio
//retorna a densidade na posicao r do volume do CT scan
double density(int r) {
	return ((double)(_scan.data[r]))/255.0;
}

//τ(d(t))
double tauDt(double t) {
	double d = density(Ray_trace(t));
	return transferFunction(d);
}

//calcula integral do expoente da função de renderização
// int(τ(d(t))dt) de 0 até s
double innerIntegral(double s) {
	double r = simpson(&tauDt, 0.0, s, _h);
	//printf(" inner s = %.2f\n", s);
	//printf("%f\n", r);

	return r;
}

//Função de renderização volumétrica
//τ(d(s))*exp(int(τ(d(t))dt))
double renderingFunction(double s) {
	//printf("rendering function s = %.2f \n", s);
	return tauDt(s)*exp(-(innerIntegral(s)));
}


double outerIntegral(int oi, int oj, int ok, int L) {
	Ray_setOrigin(oi, oj, ok);

	//printf("oi = %d, oj = %d, ok= %d \n", oi, oj, ok);
	double s = simpson(&renderingFunction, 0.0, L, _h);
	//printf(" outer\n");
	return s;
}

void normalizeValues(unsigned int size, double * values) {
	double max = 0.0;
	int i;

	for (i = 0; i < size; i++) {
		if (values[i] > max)
			max = values[i];
	}

	for (i = 0; i < size; i++) {
		values[i] /= max;
	}
}

void renderVisualization(char * filename, unsigned int * dimensions) {
	unsigned int i, k;
	unsigned int bufferSize = dimensions[0]*dimensions[2]/2;
	double * result = (double*)malloc(sizeof(double)*bufferSize);
	double value1, value2;

	_scan.nx = dimensions[0];
	_scan.ny = dimensions[1];
	_scan.nz = dimensions[2];
	_scan.data = FM_readScanFile("vridados-head-8bit.raw", _scan.nx*_scan.ny*_scan.nz);

	printf("end - read scan files\n");
	//seta parâmetros do raio
	Ray_setDirection(0, 1, 0);
	Ray_setDimensions(dimensions[0], dimensions[1], dimensions[2]);

	printf("end - set ray dimensions\n");

	for (i = 0; i < _scan.nx/2; i++) {
		for (k = 0; k < _scan.nz; k++) {
			//printf("i = %d, k = %d\n", i, k);
			value1 = outerIntegral(2*i, 0, k, _scan.ny - 1);
			value2 = outerIntegral(2*i+1, 0, k, _scan.ny - 1);
			result[_scan.nx*i/2 + k] = (value1 + value2)/2.0;
		}
	}

	printf("end - integral\n");

	//@TODO: normalizar valores de result entre 0 e 1
	normalizeValues(bufferSize, result);
	//@TODO: gerar imagem PGM
	FM_writePGMFile(result, _scan.nx/2, _scan.nz);
}