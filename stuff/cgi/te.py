#!/usr/bin/env python

import os
from PIL import Image, ImageDraw

# Set content type to image/png
print("Content-Type: image/png")
print()

# Create a new image
image = Image.new("RGB", (200, 200), "white")
draw = ImageDraw.Draw(image)

# Draw a blue circle on the image
draw.ellipse([50, 50, 150, 150], fill="blue")

# Save the image to a temporary file
temp_file = "/Users/asidqi/Desktop/tet/generated_image.png"
image.save(temp_file, format="PNG")

# Read the temporary file and output its contents
with open(temp_file, "rb") as f:
    png_data = f.read()

# Print the PNG data to the standard output
print(png_data)