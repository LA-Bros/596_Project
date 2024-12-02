#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define DIMENSIONS 2
#define POPULATION_SIZE 50
#define MAX_GENERATIONS 50
#define LOWER_BOUND -5.0
#define UPPER_BOUND 5.0
#define MUTATION_RATE 0.1
#define CROSSOVER_RATE 0.9
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

// Select a parent using tournament selection
int tournament_selection(double fitness[]) {
    int i1 = rand() % POPULATION_SIZE;
    int i2 = rand() % POPULATION_SIZE;
    return (fitness[i1] < fitness[i2]) ? i1 : i2;
}

// Perform crossover between two parents
void crossover(double parent1[], double parent2[], double child1[], double child2[]) {
    for (int d = 0; d < DIMENSIONS; d++) {
        if ((rand() / (double)RAND_MAX) < CROSSOVER_RATE) {
            double alpha = rand() / (double)RAND_MAX;
            child1[d] = alpha * parent1[d] + (1 - alpha) * parent2[d];
            child2[d] = alpha * parent2[d] + (1 - alpha) * parent1[d];
        } else {
            child1[d] = parent1[d];
            child2[d] = parent2[d];
        }
    }
}

// Perform mutation
void mutate(double individual[]) {
    for (int d = 0; d < DIMENSIONS; d++) {
        if ((rand() / (double)RAND_MAX) < MUTATION_RATE) {
            double mutation = (rand() / (double)RAND_MAX - 0.5) * 2.0;  // Random step in [-1, 1]
            individual[d] += mutation;

            // Ensure bounds
            if (individual[d] < LOWER_BOUND) individual[d] = LOWER_BOUND;
            if (individual[d] > UPPER_BOUND) individual[d] = UPPER_BOUND;
        }
    }
}

int main() {
    double population[POPULATION_SIZE][DIMENSIONS];
    double fitness[POPULATION_SIZE];
    double new_population[POPULATION_SIZE][DIMENSIONS];
    double new_fitness[POPULATION_SIZE];
    double best_solution[DIMENSIONS];
    double best_fitness = INFINITY;

    // File to store points
    FILE *file = fopen("ga_ackley_points.txt", "w");
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

    // Main GA loop
    for (int generation = 0; generation < MAX_GENERATIONS; generation++) {
        fprintf(file, "Iteration %d:\n", generation);

        #pragma omp parallel for
        for (int i = 0; i < POPULATION_SIZE; i += 2) {
            // Select parents
            int p1 = tournament_selection(fitness);
            int p2 = tournament_selection(fitness);

            // Perform crossover
            double child1[DIMENSIONS], child2[DIMENSIONS];
            crossover(population[p1], population[p2], child1, child2);

            // Perform mutation
            mutate(child1);
            mutate(child2);

            // Evaluate fitness
            double child1_fitness = ackley(child1);
            double child2_fitness = ackley(child2);

            // Store in new population
            for (int d = 0; d < DIMENSIONS; d++) {
                new_population[i][d] = child1[d];
                new_population[i + 1][d] = child2[d];
            }
            new_fitness[i] = child1_fitness;
            new_fitness[i + 1] = child2_fitness;

            // Update the file with current points
            fprintf(file, "%lf %lf %lf\n", child1[0], child1[1], child1_fitness);
            fprintf(file, "%lf %lf %lf\n", child2[0], child2[1], child2_fitness);
        }

        // Replace old population with new
        for (int i = 0; i < POPULATION_SIZE; i++) {
            for (int d = 0; d < DIMENSIONS; d++) {
                population[i][d] = new_population[i][d];
            }
            fitness[i] = new_fitness[i];

            // Update global best
            if (fitness[i] < best_fitness) {
                best_fitness = fitness[i];
                for (int d = 0; d < DIMENSIONS; d++) {
                    best_solution[d] = population[i][d];
                }
            }
        }
    }

    fclose(file);

    printf("Optimization complete.\nBest value: %lf at position (", best_fitness);
    for (int d = 0; d < DIMENSIONS; d++) {
        printf("%lf", best_solution[d]);
        if (d < DIMENSIONS - 1) printf(", ");
    }
    printf(")\n");

    return 0;
}
