double Simpson_simpson (double (*f) (double), double a, double b, double h);

double Simpson_adaptiveSimpson(double a, double b, double(*f) (double x), double tol);