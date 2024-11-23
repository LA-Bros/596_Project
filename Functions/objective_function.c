#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846

// Rastrigin function for two-dimensional input
double rastrigin_2d(double x1, double x2) {
    double A = 10.0;
    double result = 2 * A; // A * n, where n = 2 for two dimensions
    result += x1 * x1 - A * cos(2 * PI * x1);
    result += x2 * x2 - A * cos(2 * PI * x2);
    return result;
}
