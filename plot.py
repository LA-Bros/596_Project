import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Read data
points = []
with open("pso_rastrigin_points.txt", "r") as file:
    for line in file:
        if "Iteration" not in line:
            points.append(list(map(float, line.strip().split())))

# Convert to arrays
points = list(zip(*points))
x, y, z = points[0], points[1], points[2]

# Plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(x, y, z, c=z, cmap='viridis')
ax.set_xlabel("x1")
ax.set_ylabel("x2")
ax.set_zlabel("Rastrigin Value")
plt.show()
