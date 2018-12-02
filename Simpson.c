#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "Simpson.h"
#include "Ray.h"

//@TODO
//fazer simpson com passo fixo e passo adaptativo
double simpson (double (*f) (double), double a, double b, double h) {
	double sum, ti, tj;
	double fti, ftj;

	ti = a;
	tj = ti + h;
	sum = 0.0;

	fti = f(ti);

	while (tj <= b)
	{
		ftj = f(tj);
		sum += (h / 6.0) * (fti + 4.0*f((ti + tj) / 2.0) + ftj);

		fti = ftj;
		ti += h;
		tj += h;
	}

	return sum;
}

double DoubleSimpson(double a, double b, double(*f) (double x), double* v)
{
	double erro, sab, sac, scb, c;
	double fa, fb, fc;

	c = (b + a) / 2.0;
	fa = f(a);
	fb = f(b);
	fc = f(c);

	sab = ((b - a) / 6.0) * (fa + 4*fc + fb);
	sac = ((c - a) / 6.0) * (fa + 4*f((c+a)/2.0) + fc);
	scb = ((b - c) / 6.0) * (fc + 4*f((c+b)/2.0) + fb);
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

	return AdaptiveSimpson(a, c, f, tol) + AdaptiveSimpson(c, b, f, tol/2.0);
}