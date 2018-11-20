#include <stdlib.h>
#include "Simpson.h"
#include "Ray.h"

//@TODO
//fazer simpson com passo fixo e passo adaptativo
//será necessário adaptar o simpson para calcular o índice a partir da função ray
double simpson (double (*f) (double), double a, double b, double h) {
	return 0.0;
}

double simpsonN(double(*f) (double), double a, double b, int n)
{
	int i;
	double sum, h, xi, xj;

	h = (b - a) / n;
	xi = a;
	xj = xi + h;
	sum = 0.0;

	for (i = 0; i < n; i++)
	{
		sum += (h / 6.0) * (f(xi) + 4.0*f((xi + xj) / 2.0) + f(xj));
		xi += h;
		xj += h;
	}

	return sum;
}

double simpsonSimples(double a, double b, double(*f) (double x))
{
	return ((b - a) / 6.0) * (f(a) + 4 * f((a + b) / 2.0) + f(b));
}

double DoubleSimpson(double a, double b, double(*f) (double x), double* v)
{
	double erro, sab, sac, scb, c;

	c = (b + a) / 2.0;

	sab = simpsonSimples(a, b, f);
	sac = simpsonSimples(a, c, f);
	scb = simpsonSimples(c, b, f);
	erro = fabs(sab - sac - scb) / 15.0;

	*v = sac + scb - erro;
	return erro;
}

double AdaptiveSimpson(double a, double b, double(*f) (double x), double tol)
{
	double v, erro;
	double c;

	erro = DoubleSimpson(a, b, f, &v);
	c = (a + b) / 2.0;

	if (erro <= tol)
	{
		return v;
	}

	return AdaptiveSimpson(a, c, f, tol) + AdaptiveSimpson(c, b, f, tol);
}