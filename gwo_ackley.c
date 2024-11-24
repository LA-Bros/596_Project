#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define DIMENSIONS 2
#define POPULATION 50
#define ITERATIONS 100
#define LOWER_BOUND -5
#define UPPER_BOUND 5
#define PI 3.14159265358979323846

// Ackley function
double ackley(double *x) {
    double sum1 = 0.0, sum2 = 0.0;

    #pragma omp parallel for reduction(+:sum1, sum2)
    for (int i = 0; i < DIMENSIONS; i++) {
        sum1 += x[i] * x[i];
        sum2 += cos(2 * PI * x[i]);
    }

    return -20.0 * exp(-0.2 * sqrt(sum1 / DIMENSIONS)) - exp(sum2 / DIMENSIONS) + 20.0 + exp(1);
}

// Random number generator within a range
double random_in_range(double lower, double upper) {
    return lower + (rand() / (double)RAND_MAX) * (upper - lower);
}

// Initialize population
void initialize_population(double wolves[][DIMENSIONS]) {
    #pragma omp parallel for
    for (int i = 0; i < POPULATION; i++) {
        for (int d = 0; d < DIMENSIONS; d++) {
            wolves[i][d] = random_in_range(LOWER_BOUND, UPPER_BOUND);
        }
    }
}

// Update positions
void update_positions(double wolves[][DIMENSIONS], double alpha[], double beta[], double delta[], double a) {
    #pragma omp parallel for
    for (int i = 0; i < POPULATION; i++) {
        for (int d = 0; d < DIMENSIONS; d++) {
            double r1 = random_in_range(0, 1);
            double r2 = random_in_range(0, 1);
            double A1 = 2 * a * r1 - a;
            double C1 = 2 * r2;

            double D_alpha = fabs(C1 * alpha[d] - wolves[i][d]);
            double X1 = alpha[d] - A1 * D_alpha;

            r1 = random_in_range(0, 1);
            r2 = random_in_range(0, 1);
            double A2 = 2 * a * r1 - a;
            double C2 = 2 * r2;

            double D_beta = fabs(C2 * beta[d] - wolves[i][d]);
            double X2 = beta[d] - A2 * D_beta;

            r1 = random_in_range(0, 1);
            r2 = random_in_range(0, 1);
            double A3 = 2 * a * r1 - a;
            double C3 = 2 * r2;

            double D_delta = fabs(C3 * delta[d] - wolves[i][d]);
            double X3 = delta[d] - A3 * D_delta;

            wolves[i][d] = (X1 + X2 + X3) / 3.0;

            // Bound the position within the search space
            if (wolves[i][d] < LOWER_BOUND) wolves[i][d] = LOWER_BOUND;
            if (wolves[i][d] > UPPER_BOUND) wolves[i][d] = UPPER_BOUND;
        }
    }
}

int main() {
    double wolves[POPULATION][DIMENSIONS];
    double alpha[DIMENSIONS], beta[DIMENSIONS], delta[DIMENSIONS];
    double alpha_score = INFINITY, beta_score = INFINITY, delta_score = INFINITY;

    // Initialize population
    initialize_population(wolves);

    // Main optimization loop
    for (int iter = 0; iter < ITERATIONS; iter++) {
        #pragma omp parallel for
        for (int i = 0; i < POPULATION; i++) {
            double fitness = ackley(wolves[i]);

            #pragma omp critical
            {
                if (fitness < alpha_score) {
                    alpha_score = fitness;
                    for (int d = 0; d < DIMENSIONS; d++) {
                        alpha[d] = wolves[i][d];
                    }
                } else if (fitness < beta_score) {
                    beta_score = fitness;
                    for (int d = 0; d < DIMENSIONS; d++) {
                        beta[d] = wolves[i][d];
                    }
                } else if (fitness < delta_score) {
                    delta_score = fitness;
                    for (int d = 0; d < DIMENSIONS; d++) {
                        delta[d] = wolves[i][d];
                    }
                }
            }
        }

        // Update positions
        double a = 2.0 - iter * (2.0 / ITERATIONS); // Linear reduction of 'a' from 2 to 0
        update_positions(wolves, alpha, beta, delta, a);

        printf("Iteration %d: Alpha score = %lf\n", iter, alpha_score);
    }

    printf("Optimization complete.\nBest solution found: %lf at position (", alpha_score);
    for (int d = 0; d < DIMENSIONS; d++) {
        printf("%lf", alpha[d]);
        if (d < DIMENSIONS - 1) printf(", ");
    }
    printf(")\n");

    return 0;
}
