double simpson (double (*f) (double), double a, double b, double h);
double simpsonN(double(*f) (double), double a, double b, double n);
double DoubleSimpson(double a, double b, double(*f) (double x), double* v);
double AdaptiveSimpson(double a, double b, double(*f) (double x), double tol);