#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define DIMENSIONS 2
#define PARTICLES 50
#define ITERATIONS 50
#define LOWER_BOUND -5
#define UPPER_BOUND 5
#define PI 3.14159265358979323846

// Ackley function
double ackley(double *x) {
    double sum1 = 0.0, sum2 = 0.0;
    for (int i = 0; i < DIMENSIONS; i++) {
        sum1 += x[i] * x[i];
        sum2 += cos(2 * PI * x[i]);
    }
    return -20.0 * exp(-0.2 * sqrt(sum1 / DIMENSIONS)) - exp(sum2 / DIMENSIONS) + 20.0 + exp(1);
}

// Initialize particles
void initialize_particles(double particles[][DIMENSIONS], double velocities[][DIMENSIONS], double pbest_positions[][DIMENSIONS], double *pbest_values, double *gbest_position, double *gbest_value) {
    #pragma omp parallel for
    for (int i = 0; i < PARTICLES; i++) {
        for (int d = 0; d < DIMENSIONS; d++) {
            particles[i][d] = LOWER_BOUND + (rand() / (double)RAND_MAX) * (UPPER_BOUND - LOWER_BOUND);
            velocities[i][d] = (rand() / (double)RAND_MAX) * (UPPER_BOUND - LOWER_BOUND) * 0.1;
            pbest_positions[i][d] = particles[i][d];
        }
        pbest_values[i] = ackley(particles[i]);
        #pragma omp critical
        {
            if (pbest_values[i] < *gbest_value) {
                *gbest_value = pbest_values[i];
                for (int d = 0; d < DIMENSIONS; d++) {
                    gbest_position[d] = particles[i][d];
                }
            }
        }
    }
}

int main() {
    double particles[PARTICLES][DIMENSIONS];
    double velocities[PARTICLES][DIMENSIONS];
    double pbest_positions[PARTICLES][DIMENSIONS];
    double pbest_values[PARTICLES];
    double gbest_position[DIMENSIONS];
    double gbest_value = INFINITY;

    double inertia = 0.7, c1 = 1.5, c2 = 1.5;

    // File to store points
    FILE *file = fopen("pso_ackley_points.txt", "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return 1;
    }

    // Initialize particles
    initialize_particles(particles, velocities, pbest_positions, pbest_values, gbest_position, &gbest_value);

    // Run iterations
    for (int iter = 0; iter <= ITERATIONS; iter++) {
        fprintf(file, "Iteration %d:\n", iter);
        #pragma omp parallel for
        for (int i = 0; i < PARTICLES; i++) {
            double new_value;
            for (int d = 0; d < DIMENSIONS; d++) {
                velocities[i][d] = inertia * velocities[i][d] +
                                   c1 * ((rand() / (double)RAND_MAX) * (pbest_positions[i][d] - particles[i][d])) +
                                   c2 * ((rand() / (double)RAND_MAX) * (gbest_position[d] - particles[i][d]));

                particles[i][d] += velocities[i][d];

                // Keep particles within bounds
                if (particles[i][d] < LOWER_BOUND) particles[i][d] = LOWER_BOUND;
                if (particles[i][d] > UPPER_BOUND) particles[i][d] = UPPER_BOUND;
            }

            new_value = ackley(particles[i]);

            // Update personal best
            if (new_value < pbest_values[i]) {
                pbest_values[i] = new_value;
                for (int d = 0; d < DIMENSIONS; d++) {
                    pbest_positions[i][d] = particles[i][d];
                }
            }

            // Update global best
            #pragma omp critical
            {
                if (new_value < gbest_value) {
                    gbest_value = new_value;
                    for (int d = 0; d < DIMENSIONS; d++) {
                        gbest_position[d] = particles[i][d];
                    }
                }
            }

            // Write particle position and fitness value to file
            fprintf(file, "%lf %lf %lf\n", particles[i][0], particles[i][1], new_value);
        }
    }

    fclose(file);

    printf("Optimization complete.\nGlobal best value: %lf at position (", gbest_value);
    for (int d = 0; d < DIMENSIONS; d++) {
        printf("%lf", gbest_position[d]);
        if (d < DIMENSIONS - 1) printf(", ");
    }
    printf(")\n");

    return 0;
}