#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define DIMENSIONS 2
#define POPULATION_SIZE 50
#define ITERATIONS 50
#define LOWER_BOUND -5.0
#define UPPER_BOUND 5.0
#define CR 0.9          // Crossover probability
#define F 0.8           // Mutation scaling factor
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

// Initialize population
void initialize_population(double population[][DIMENSIONS], double fitness[]) {
    #pragma omp parallel for
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int d = 0; d < DIMENSIONS; d++) {
            population[i][d] = LOWER_BOUND + (rand() / (double)RAND_MAX) * (UPPER_BOUND - LOWER_BOUND);
        }
        fitness[i] = ackley(population[i]);
    }
}

// Select three distinct individuals for mutation
void select_random_indices(int *r1, int *r2, int *r3, int current) {
    do {
        *r1 = rand() % POPULATION_SIZE;
    } while (*r1 == current);
    do {
        *r2 = rand() % POPULATION_SIZE;
    } while (*r2 == *r1 || *r2 == current);
    do {
        *r3 = rand() % POPULATION_SIZE;
    } while (*r3 == *r1 || *r3 == *r2 || *r3 == current);
}

int main() {
    double population[POPULATION_SIZE][DIMENSIONS];
    double fitness[POPULATION_SIZE];
    double best_solution[DIMENSIONS];
    double best_fitness = INFINITY;

    // File to store points
    FILE *file = fopen("de_ackley_points.txt", "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return 1;
    }

    // Initialize population
    initialize_population(population, fitness);

    // Find initial best solution
    for (int i = 0; i < POPULATION_SIZE; i++) {
        if (fitness[i] < best_fitness) {
            best_fitness = fitness[i];
            for (int d = 0; d < DIMENSIONS; d++) {
                best_solution[d] = population[i][d];
            }
        }
    }

    // Main DE loop
    for (int iter = 0; iter < ITERATIONS; iter++) {
        fprintf(file, "Iteration %d:\n", iter);
        double new_population[POPULATION_SIZE][DIMENSIONS];
        double new_fitness[POPULATION_SIZE];

        #pragma omp parallel for
        for (int i = 0; i < POPULATION_SIZE; i++) {
            // Mutation and crossover
            int r1, r2, r3;
            select_random_indices(&r1, &r2, &r3, i);
            double trial_vector[DIMENSIONS];

            for (int d = 0; d < DIMENSIONS; d++) {
                if ((rand() / (double)RAND_MAX) < CR) {
                    trial_vector[d] = population[r1][d] + F * (population[r2][d] - population[r3][d]);
                } else {
                    trial_vector[d] = population[i][d];
                }

                // Ensure the trial vector is within bounds
                if (trial_vector[d] < LOWER_BOUND) trial_vector[d] = LOWER_BOUND;
                if (trial_vector[d] > UPPER_BOUND) trial_vector[d] = UPPER_BOUND;
            }

            // Evaluate trial vector
            double trial_fitness = ackley(trial_vector);

            // Selection
            if (trial_fitness < fitness[i]) {
                for (int d = 0; d < DIMENSIONS; d++) {
                    new_population[i][d] = trial_vector[d];
                }
                new_fitness[i] = trial_fitness;
            } else {
                for (int d = 0; d < DIMENSIONS; d++) {
                    new_population[i][d] = population[i][d];
                }
                new_fitness[i] = fitness[i];
            }

            // Update the file with current points
            fprintf(file, "%lf %lf %lf\n", new_population[i][0], new_population[i][1], new_fitness[i]);
        }

        // Update population and fitness
        for (int i = 0; i < POPULATION_SIZE; i++) {
            for (int d = 0; d < DIMENSIONS; d++) {
                population[i][d] = new_population[i][d];
            }
            fitness[i] = new_fitness[i];
        }

        // Update global best
        for (int i = 0; i < POPULATION_SIZE; i++) {
            if (fitness[i] < best_fitness) {
                best_fitness = fitness[i];
                for (int d = 0; d < DIMENSIONS; d++) {
                    best_solution[d] = population[i][d];
                }
            }
        }
    }

    fclose(file);

    printf("Optimization complete.\nGlobal best fitness: %lf at position (", best_fitness);
    for (int d = 0; d < DIMENSIONS; d++) {
        printf("%lf", best_solution[d]);
        if (d < DIMENSIONS - 1) printf(", ");
    }
    printf(")\n");

    return 0;
}
