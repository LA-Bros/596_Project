# Target Function
rastrigin function\
![](https://github.com/LA-Bros/596_Project/blob/main/Image/Function.jpg?raw=true)


# Algorithms
Genetic Algorithm (GA)\
(GWO)\
(DE)\
(SA)

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
![](https://github.com/LA-Bros/596_Project/blob/main/Image/PSO_Rastrigin_Iterations_Final.gif?raw=true)
