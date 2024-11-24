# Target Function
Ackley function\
![](https://github.com/LA-Bros/596_Project/blob/main/Image/Ackley.jpg?raw=true)


# Algorithms
Genetic Algorithm (GA)\
Grey Wolf Optimization (GWO)\
Differential Evolution (DE)\
Particle Swarm Optimization (PSO)

# OpenMP
Parallel computing iterative process

```python
  for (iter = 0; iter < MAX_ITER; iter++) {
        // Leverage OpenMP to parallel computing iterative process
        #pragma omp parallel for
        for (i = 0; i < POP_SIZE; i++) {
            fitness[i] = rastrigin_function(population[i], DIM);
        }
```
# Visualization
Show position of points in each iteration\

PSO\
![](https://github.com/LA-Bros/596_Project/blob/main/Image/PSO_Ackley.gif?raw=true)

GA\
![](https://github.com/LA-Bros/596_Project/blob/main/Image/GA_Ackley.gif?raw=true)

DE\
![](https://github.com/LA-Bros/596_Project/blob/main/Image/DE_Ackley.gif?raw=true)

GWO\
![](https://github.com/LA-Bros/596_Project/blob/main/Image/GWO_Ackley.gif?raw=true)


