double simpson (double (*f) (double), double a, double b, double h);

double AdaptiveSimpson(double a, double b, double(*f) (double x), double tol);