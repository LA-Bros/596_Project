import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import imageio.v2 as imageio
import os

# Ensure the output directory exists
os.makedirs("Image", exist_ok=True)


# Define the Rastrigin function
def rastrigin(X, Y):
    return 10 * 2 + (X ** 2 - 10 * np.cos(2 * np.pi * X)) + (Y ** 2 - 10 * np.cos(2 * np.pi * Y))


# Reload data from the truncated file
filename = "pso_rastrigin_points.txt"
iterations = []
points = []

with open(filename, 'r') as file:
    current_iter = []
    for line in file:
        if "Iteration" in line:
            if current_iter:
                points.append(current_iter)
                current_iter = []
            iterations.append(line.strip())
        else:
            current_iter.append(list(map(float, line.strip().split())))
    if current_iter:
        points.append(current_iter)

# Convert points to NumPy arrays for easier handling
points = [np.array(p) for p in points]

# Create the grid for the Rastrigin function
x = np.linspace(-5.12, 5.12, 100)
y = np.linspace(-5.12, 5.12, 100)
X, Y = np.meshgrid(x, y)
Z = rastrigin(X, Y)

# Create a list to store frames for the GIF
frames = []

# Plot each iteration and save as a frame
for i, iteration_points in enumerate(points):
    fig = plt.figure(figsize=(10, 7))
    ax = fig.add_subplot(111, projection='3d')
    ax.plot_surface(X, Y, Z, cmap='viridis', alpha=0.3)  # Make the function less prominent

    # Ensure iteration_points has the correct shape
    if iteration_points.shape[1] == 3:  # Should be (n_points, 3)
        ax.scatter(iteration_points[:, 0], iteration_points[:, 1], iteration_points[:, 2],
                   c='red', s=50, marker='o', label=f"Iteration {i}")  # Use circular markers for points
    else:
        print(f"Warning: Skipping iteration {i} due to mismatched data dimensions.")

    ax.set_xlabel('X1')
    ax.set_ylabel('X2')
    ax.set_zlabel('Rastrigin Value')
    ax.set_title(f'PSO on Rastrigin Function - Iteration {i}')
    plt.legend()

    # Save frame to buffer
    frame_path = f"Image/frame_{i}.png"
    plt.savefig(frame_path)
    frames.append(imageio.imread(frame_path))
    plt.close()

# Save the updated frames as a GIF
gif_path = "Image/PSO_Rastrigin_Iterations_Final.gif"
imageio.mimsave(gif_path, frames, duration=1.0, loop=0)

print(f"GIF saved successfully to {gif_path}")
