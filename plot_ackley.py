import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import imageio.v2 as imageio
import io


# Define the Ackley function
def ackley(X, Y):
    return -20.0 * np.exp(-0.2 * np.sqrt(0.5 * (X ** 2 + Y ** 2))) \
           - np.exp(0.5 * (np.cos(2 * np.pi * X) + np.cos(2 * np.pi * Y))) \
           + 20.0 + np.e


# Load data from the PSO output file
filenames = ["Function/PSO/pso_ackley_points.txt", "Function/GA/ga_ackley_points.txt", "Function/DE/de_ackley_points.txt", "Function/GWO/gwo_ackley_points.txt"]
gifnames = ["Image/PSO_Ackley.gif", "Image/GA_Ackley.gif", "Image/DE_Ackley.gif", "Image/GWO_Ackley.gif"]
iterations = []
points = []

for j in range(0, 4):
    with open(filenames[j], 'r') as file:
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
    points = points[:30]
    # Convert points to NumPy arrays for easier handling
    points = [np.array(p) for p in points]

    # Create the grid for the Ackley function
    x = np.linspace(-5, 5, 100)
    y = np.linspace(-5, 5, 100)
    X, Y = np.meshgrid(x, y)
    Z = ackley(X, Y)

    # Create a list to store frames for the GIF
    frames = []

    # Plot each iteration and save as a frame
    for i, iteration_points in enumerate(points):
        fig = plt.figure(figsize=(10, 7))
        ax = fig.add_subplot(111, projection='3d')
        ax.plot_surface(X, Y, Z, cmap='viridis', alpha=0.3)  # Ackley surface is less prominent
        ax.scatter(iteration_points[:, 0], iteration_points[:, 1], iteration_points[:, 2],
                   c='red', s=50, marker='o', label=f"Iteration {i}")  # Points are circles for visibility
        ax.set_xlabel('X1')
        ax.set_ylabel('X2')
        ax.set_zlabel('Ackley Value')
        ax.set_title(f'GWO on Ackley Function - Iteration {i}')
        plt.legend()

        # Save frame to buffer
        buffer = io.BytesIO()
        plt.savefig(buffer, format='png')  # Save the figure to buffer
        buffer.seek(0)
        frames.append(imageio.imread(buffer))
        buffer.close()
        plt.close()

    # Save the frames as a GIF
    gif_path = gifnames[j]
    imageio.mimsave(gif_path, frames, fps=8, loop=0)



    print(f"GIF saved as {gif_path}")
