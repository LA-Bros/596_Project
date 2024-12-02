# Project Description
Our project focuses on selecting a benchmark function and visualizing the optimization process of four algorithms on this function. Additionally, we aim to leverage OpenMP to parallelize the computation and enhance the iteration efficiency of these algorithms. After careful consideration, we chose the Ackley function as the benchmark function, and the four optimization algorithms are [Genetic Algorithm (GA)](https://en.wikipedia.org/wiki/Genetic_algorithm), [Grey Wolf Optimizer (GWO)](https://en.wikiversity.org/wiki/Algorithm_models/Grey_Wolf_Optimizer), [Differential Evolution (DE)](https://en.wikipedia.org/wiki/Differential_evolution), and [Particle Swarm Optimization (PSO)](https://en.wikipedia.org/wiki/Particle_swarm_optimization).



# Benchmark Function
## Ackley function
### $$f(\mathbf{x}) = -a \cdot \exp\left(-b \cdot \sqrt{\frac{1}{n} \sum_{i=1}^n x_i^2}\right) - \exp\left(\frac{1}{n} \sum_{i=1}^n \cos(c \cdot x_i)\right) + a + \exp(1)$$
#### Properties
1. **Global Minimum**:
   - The global minimum value is $\(f(\mathbf{x}) = 0\)$, located at $\(\mathbf{x} = (0, 0, \ldots, 0)\)$.

2. **Multi-Modality**:
   - The function has numerous local minima, making it difficult for optimization algorithms to find the global minimum.

3. **Use in Benchmarking**:
   - Ackley function is often used to evaluate optimization algorithms like Particle Swarm Optimization (PSO), Genetic Algorithm (GA), Differential Evolution (DE), and more.

#### Visualization
The Ackley function in three dimensions is typically visualized as a smooth surface with a central deep valley at $\((0, 0, 0)\)$, representing the global minimum.

![](https://github.com/LA-Bros/596_Project/blob/main/Image/Ackley.jpg?raw=true)




# Algorithms
### Genetic Algorithm (GA)
Genetic Algorithm (GA) is inspired by the process of natural selection, mimicking biological evolution. It involves populations of candidate solutions that evolve through selection, crossover, and mutation to find optimal solutions.
### Grey Wolf Optimization (GWO)
Grey Wolf Optimization (GWO) is a nature-inspired algorithm based on the social hierarchy and hunting behavior of grey wolves. The wolves form a pack led by the alpha, beta, and delta wolves, with the remaining wolves acting as followers (omega).
### Differential Evolution (DE)
Differential Evolution (DE) is a population-based optimization algorithm that relies on the differences between randomly selected individuals to generate new candidate solutions.
### Particle Swarm Optimization (PSO)
Particle Swarm Optimization (PSO) is inspired by the social behavior of birds flocking or fish schooling. Each particle represents a potential solution and moves through the search space based on its own experience and the experience of its neighbors.

# OpenMP
## 1. Particle Swarm Optimization (PSO)
### Parallelization Points:

1. Fitness Evaluation:
- Evaluate the fitness of all particles in parallel using #pragma omp parallel for.
2. Position and Velocity Updates:
- Each particle updates its position and velocity independently. Use #pragma omp parallel for to parallelize these updates.
Parallel computing iterative process

## 2. Genetic Algorithm (GA)
### Parallelization Points:

1. Fitness Evaluation:
- Evaluating the fitness of each individual in the population is computationally independent. Use #pragma omp parallel for to compute the fitness for all individuals simultaneously.
2. Crossover and Mutation:
- Each pair of parents produces offspring independently. Use #pragma omp parallel for to perform crossover and mutation in parallel for different pairs of parents.
3. Selection:
- Tournament selection or other selection mechanisms can be parallelized for each individual since selections do not depend on other individuals.


## 3. Differential Evolution (DE)
### Parallelization Points:

1. Mutation and Crossover:

- The creation of trial vectors (mutation + crossover) for each individual is independent. Use #pragma omp parallel for to parallelize the generation of trial vectors.
2. Fitness Evaluation:

- Evaluate the fitness of trial vectors in parallel since fitness computation is independent for each vector.

## 4. Grey Wolf Optimization (GWO)
### Parallelization Points:

1. Fitness Evaluation:
- Evaluating the fitness of each wolf in the population can be parallelized using #pragma omp parallel for.
2. Position Updates:
- In GWO, the positions of wolves are updated based on the alpha, beta, and delta wolves' positions. Each wolf's position update is independent and can be parallelized.



## Example
```python
#pragma omp parallel for
for (int i = 0; i < POPULATION_SIZE; i += 2) {
    // Select parents
    int p1 = tournament_selection(fitness);
    int p2 = tournament_selection(fitness);

    // Crossover and mutation
    double child1[DIMENSIONS], child2[DIMENSIONS];
    crossover(population[p1], population[p2], child1, child2);
    mutate(child1);
    mutate(child2);

    // Evaluate fitness
    new_fitness[i] = ackley(child1);
    new_fitness[i + 1] = ackley(child2);
}
```
# Visualization
Show position of points in each iteration
![](https://github.com/LA-Bros/596_Project/blob/main/Image/Combined_Ackley.gif?raw=true)



PSO\
![](https://github.com/LA-Bros/596_Project/blob/main/Image/PSO_Ackley.gif?raw=true)

GA\
![](https://github.com/LA-Bros/596_Project/blob/main/Image/GA_Ackley.gif?raw=true)

DE\
![](https://github.com/LA-Bros/596_Project/blob/main/Image/DE_Ackley.gif?raw=true)

GWO\
![](https://github.com/LA-Bros/596_Project/blob/main/Image/GWO_Ackley.gif?raw=true)





