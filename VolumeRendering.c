#include "VolumeRendering.h"
#include "Ray.h"
#include "FileManager.h"
#include "Simpson.h"
#include <stdlib.h>
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
	return (double) _scan.data[r];
}

//τ(d(t))
double tauDt(double t) {
	double d = density(t);
	return transferFunction(d);
}

//calcula integral do expoente da função de renderização
// int(τ(d(t))dt) de 0 até s
double innerIntegral(double s) {
	return simpson(&tauDt, 0.0, s, _h);
}

//Função de renderização volumétrica
//τ(d(s))*exp(int(τ(d(t))dt))
double renderingFunction(double s) {
	double d = density(s);
	return tauDt(d)*exp(-(innerIntegral(s)));
}


double outerIntegral(int oi, int oj, int ok, int L) {
	Ray_setOrigin(oi, oj, ok);

	return simpson(&renderingFunction, 0.0, L, _h);
}


void renderVisualization(char * filename, unsigned int * dimensions) {
	int i, k;
	double * result = (double*)malloc(sizeof(double)*dimensions[0]*dimensions[2]/2);
	double value1, value2;

	_scan.nx = dimensions[0];
	_scan.ny = dimensions[1];
	_scan.nz = dimensions[2];
	_scan.data = FM_readScanFile("vridados-head-8bit.raw", _scan.nx*_scan.ny*_scan.nz);

	//seta parâmetros do raio
	Ray_setDirection(0, 1, 0);
	Ray_setDimensions(dimensions[0], dimensions[1], dimensions[2]);


	for (i = 0; i < dimensions[0]/2; i++) {
		for (k = 0; k < dimensions[2]; k++) {
			value1 = outerIntegral(2*i, 0, k, dimensions[1]);
			value2 = outerIntegral(2*i+1, 0, k, dimensions[1]);

			result[dimensions[0]*i/2 + k] = (value1 + value2)/2.0;
		}
	}

	//@TODO: normalizar valores de result entre 0 e 1
	//@TODO: gerar imagem PGM
}