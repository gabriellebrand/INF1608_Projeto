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

//esse simpson adaptativo está mais rápido do que o anterior
double DoubleSimpson (double a, double b, double (*f) (double x), double* v) {
	double error = 0, h = b-a, c = (a+b)/2.;
	double fx, fx_2, fa, fb, fc;
	fa = f(a);
	fb = f(b);
	fc = f(c);
	
	//fx = S[a,b]
	//fx_2 = S[a,c] + S[c,b]
	
	fx = (h/6.)*(fa + 4.*fc + fb);
	fx_2 = (h/12.)*(fa + 4.*(f((a+c)/2.) + f((c+b)/2.)) + 2.*fc + fb);
	
	error = fabs(fx - fx_2)/15.;
	*v = fx_2 - error;
	
	return error;
}

double AdaptiveSimpson (double a, double b, double (*f) (double x), double tol) {
	double res, error, m;
	
	m = (a+b)/2.;
	error = DoubleSimpson(a, b, f, &res);

	if (error > 15.0*tol)
		 return AdaptiveSimpson(a, m, f, tol/2.) + AdaptiveSimpson(m, b, f, tol/2.);
	else
		return res;
}


// double simpsonSimples(double a, double b, double(*f) (double x))
// {
// 	return ((b - a) / 6.0) * (f(a) + 4 * f((a + b) / 2.0) + f(b));
// }

// double DoubleSimpson(double a, double b, double(*f) (double x), double* v)
// {
// 	double erro, sab, sac, scb, c;

// 	c = (b + a) / 2.0;

// 	sab = simpsonSimples(a, b, f);
// 	sac = simpsonSimples(a, c, f);
// 	scb = simpsonSimples(c, b, f);
// 	erro = fabs(sab - sac - scb) / 15.0;

// 	*v = sac + scb - erro;
// 	return erro;
// }

// double AdaptiveSimpson(double a, double b, double(*f) (double x), double tol)
// {
// 	double v, erro;
// 	double c;

// 	erro = DoubleSimpson(a, b, f, &v);
// 	c = (a + b) / 2.0;

// 	if (erro <= tol)
// 	{
// 		return v;
// 	}

// 	return AdaptiveSimpson(a, c, f, tol) + AdaptiveSimpson(c, b, f, tol);
// }