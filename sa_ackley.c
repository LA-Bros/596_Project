#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define DIMENSIONS 2
#define ITERATIONS 50
#define LOWER_BOUND -5
#define UPPER_BOUND 5
#define PI 3.14159265358979323846
#define INITIAL_TEMPERATURE 100.0
#define COOLING_RATE 0.99

// Ackley function
double ackley(double *x) {
    double sum1 = 0.0, sum2 = 0.0;
    for (int i = 0; i < DIMENSIONS; i++) {
        sum1 += x[i] * x[i];
        sum2 += cos(2 * PI * x[i]);
    }
    return -20.0 * exp(-0.2 * sqrt(sum1 / DIMENSIONS)) - exp(sum2 / DIMENSIONS) + 20.0 + exp(1);
}

// Generate a random number within a range
double random_in_range(double lower, double upper) {
    return lower + ((double)rand() / RAND_MAX) * (upper - lower);
}

// Perturb the current position
void perturb(double *position, double *new_position) {
    for (int d = 0; d < DIMENSIONS; d++) {
        double perturbation = random_in_range(-0.5, 0.5);
        new_position[d] = position[d] + perturbation;

        // Keep within bounds
        if (new_position[d] < LOWER_BOUND) new_position[d] = LOWER_BOUND;
        if (new_position[d] > UPPER_BOUND) new_position[d] = UPPER_BOUND;
    }
}

int main() {
    double current_position[DIMENSIONS];
    double best_position[DIMENSIONS];
    double new_position[DIMENSIONS];

    double current_value, new_value, best_value;
    double temperature = INITIAL_TEMPERATURE;

    // Initialize current position randomly
    for (int d = 0; d < DIMENSIONS; d++) {
        current_position[d] = random_in_range(LOWER_BOUND, UPPER_BOUND);
        best_position[d] = current_position[d];
    }

    current_value = ackley(current_position);
    best_value = current_value;

    // File to store points
    FILE *file = fopen("sa_ackley_points.txt", "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return 1;
    }

    // Main SA loop
    for (int iter = 0; iter < ITERATIONS; iter++) {
        fprintf(file, "Iteration %d: %lf %lf %lf\n", iter, current_position[0], current_position[1], current_value);

        // Generate new position
        perturb(current_position, new_position);
        new_value = ackley(new_position);

        // Decide whether to accept the new position
        if (new_value < current_value || exp((current_value - new_value) / temperature) > ((double)rand() / RAND_MAX)) {
            // Accept new position
            for (int d = 0; d < DIMENSIONS; d++) {
                current_position[d] = new_position[d];
            }
            current_value = new_value;

            // Update best position
            if (new_value < best_value) {
                best_value = new_value;
                for (int d = 0; d < DIMENSIONS; d++) {
                    best_position[d] = new_position[d];
                }
            }
        }

        // Reduce temperature
        temperature *= COOLING_RATE;
    }

    fclose(file);

    printf("Optimization complete.\nBest value: %lf at position (", best_value);
    for (int d = 0; d < DIMENSIONS; d++) {
        printf("%lf", best_position[d]);
        if (d < DIMENSIONS - 1) printf(", ");
    }
    printf(")\n");

    return 0;
}
