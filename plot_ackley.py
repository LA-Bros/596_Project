import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import imageio.v2 as imageio
import io
from PIL import Image


# Define the Ackley function
def ackley(X, Y):
    return -20.0 * np.exp(-0.2 * np.sqrt(0.5 * (X ** 2 + Y ** 2))) \
           - np.exp(0.5 * (np.cos(2 * np.pi * X) + np.cos(2 * np.pi * Y))) \
           + 20.0 + np.e


# Load data from the PSO output file
filenames = ["Function/PSO/pso_ackley_points.txt", "Function/GA/ga_ackley_points.txt", "Function/DE/de_ackley_points.txt", "Function/GWO/gwo_ackley_points.txt"]
algonames = ["PSO", "GA", "DE", "GWO"]


for j in range(0, 4):
    iterations = []
    points = []

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
        ax.set_title(f'{algonames[j]} on Ackley Function - Iteration {i}')
        plt.legend()

        # Save frame to buffer
        buffer = io.BytesIO()
        plt.savefig(buffer, format='png')  # Save the figure to buffer
        buffer.seek(0)
        # Convert to RGB format
        img = Image.open(buffer)  # Load as a PIL image
        if img.mode != "RGB":  # Ensure it's in RGB mode
            img = img.convert("RGB")
        frames.append(np.array(img))
        buffer.close()
        plt.close()

    # Save the frames as a GIF
    gif_path = f"Image/{algonames[j]}_Ackley.gif"
    imageio.mimsave(gif_path, frames, fps=6, loop=0)

    print(f"GIF saved as {gif_path}")


gifnames = ["Image/PSO_Ackley.gif", "Image/GA_Ackley.gif", "Image/DE_Ackley.gif", "Image/GWO_Ackley.gif"]
combined_gif = "Image/Combined_Ackley.gif"

gif_frames = [imageio.mimread(gif) for gif in gifnames]

frame_counts = [len(frames) for frames in gif_frames]
min_frames = min(frame_counts)

aligned_frames = [frames[:min_frames] for frames in gif_frames]

combined_frames = []
for i in range(min_frames):
    frames = [Image.fromarray(frame) for frame in (aligned_frames[j][i] for j in range(len(gifnames)))]
    # frames_resized = [frame.resize((1000, 700)) for frame in frames]
    top_row = np.hstack([frames[0], frames[1]])
    bottom_row = np.hstack([frames[2], frames[3]])
    combined_frame = np.vstack([top_row, bottom_row])
    combined_frames.append(combined_frame)

imageio.mimsave(combined_gif, combined_frames, fps=6, loop=0)
print(f"Combined GIF saved as {combined_gif}")