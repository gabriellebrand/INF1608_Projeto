#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "Simpson.h"
#include "Ray.h"

double Simpson_simpson (double (*f) (double), double a, double b, double h) {
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

double doubleSimpson(double a, double b, double(*f) (double x), double* v)
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
	erro = (sab - sac - scb) / 15.0;

	*v = sac + scb - erro;
	return fabs(erro);
}

double Simpson_adaptiveSimpson(double a, double b, double(*f) (double x), double tol)
{
	double v, erro;
	double c;

	erro = doubleSimpson(a, b, f, &v);
	c = (a + b) / 2.0;

	if (erro <= tol)
	{
		return v;
	}

	return Simpson_adaptiveSimpson(a, c, f, tol/2.0) + Simpson_adaptiveSimpson(c, b, f, tol/2.0);
}