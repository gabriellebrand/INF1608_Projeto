#include "VolumeRendering.h"
#include "Ray.h"
#include "FileManager.h"
#include "Simpson.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

//estrutura para guardar os valores do CT scan
static CT _scan;

//passo de integração
static double _h = 4.5;

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
	return _scan.data[r];
}

//τ(d(t))
double tauDt(double t) {
	//faz a interpolação e entre duas densidades
	int floor, ceil;
	double dFloor, dCeil, p, d;
	floor = (int)t;
	ceil = floor + 1.0;
	p = t - (double)floor;

	/*se o p for zero, o valor é "inteiro" e não é necessário interpolar
	  essa verificação é necessária para evitar que ocorra um seg fault
	  no adaptativo quando o t corresponde a ultima amostra */
	if (p <= 1.11e-16) {
		d = density(Ray_trace(floor));
	}
	else {
		dFloor = density(Ray_trace(floor));
		dCeil = density(Ray_trace(ceil));

		d = (1.0 - p)*dFloor + p*dCeil;
	}

	return transferFunction(d);
}

//calcula integral do expoente da função de renderização
// int(τ(d(t))dt) de 0 até s
double innerIntegral(double s) {
	return Simpson_simpson(&tauDt, 0.0, s, _h);
}

//Função de renderização volumétrica
//τ(d(s))*exp(int(τ(d(t))dt))
double renderingFunction(double s) {
	return tauDt(s)*exp(-(innerIntegral(s)));
}


double outerIntegral(int oi, int oj, int ok, int L) {
	Ray_setOrigin(oi, oj, ok);

	double s = Simpson_simpson(&renderingFunction, 0.0, L, _h);
	return s;
}

//normaliza valores entre 0 e 1
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

//converte o buffer de unsigned char [0,255] para double [0,1]
double * getDoubleData(unsigned char * values, unsigned int size) {
	int i;
	double *out = (double*)malloc(sizeof(double)*size);
	
	for (i = 0; i < size; i++)
		out[i] = ((double)values[i])/255.0;

	return out;
}

void VR_renderVisualization(char * filename, unsigned int * dimensions) {
	unsigned int i, k, outputSize, fileSize;
	unsigned char *input;
	double *result;
	double value1, value2;
	clock_t start_t, end_t;
	double total_t;

	printf("fazendo leitura do arquivo...\n");
	outputSize = dimensions[0]*dimensions[2]/2;
	fileSize = dimensions[0]*dimensions[1]*dimensions[2];

	//faz a leitura do input de dados
	input = FM_readScanFile("vridados-head-8bit.raw", fileSize);

	_scan.nx = dimensions[0];
	_scan.ny = dimensions[1];
	_scan.nz = dimensions[2];
	_scan.data = getDoubleData(input, fileSize);

	//seta parâmetros do raio
	Ray_setDirection(0, 1, 0);
	Ray_setDimensions(_scan.nx, _scan.ny, _scan.nz);

	result = (double*)malloc(sizeof(double)*outputSize);

	printf("calculando integral...\n");
	start_t = clock();
	for (k = 0; k < _scan.nz; k++) {
		for (i = 0; i < _scan.nx/2; i++) {
			value1 = outerIntegral(2*i, 0, k, _scan.ny - 1);
			value2 = outerIntegral(2*i+1, 0, k, _scan.ny - 1);
			result[_scan.nx*k/2 + i] = (value1 + value2)/2.0;
		}
	}
	end_t = clock();
	printf("start_t = %ld\n", start_t);
	printf("end_t = %ld\n", end_t);
	total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
	printf("Tempo total: %f\n", total_t);

	printf("gerando arquivo de saída...\n");
	normalizeValues(outputSize, result);
	FM_writePGMFile(result, _scan.nx/2, _scan.nz);

	free(input);
	free(result);
	free(_scan.data);
}