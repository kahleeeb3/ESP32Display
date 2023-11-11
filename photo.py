import tkinter as tk
from tkinter import filedialog
from PIL import Image, ImageTk

# Constants for the OLED dimensions
OLED_WIDTH = 128
OLED_HEIGHT = 64
THRESHOLD = 115  # Initial threshold value
image_path = None  # Store the currently opened image path

def resize_image(image_path):
    # Use the updated THRESHOLD value
    image = Image.open(image_path)
    image = image.resize(
        (OLED_WIDTH, OLED_HEIGHT), Image.LANCZOS
    )  # Change the size here
    image = image.convert("L")  # Convert to grayscale
    image = image.point(lambda p: p > threshold_var.get() and 255)  # Apply threshold
    image = image.convert("1")  # Convert to 1-bit monochrome (black and white)
    return image

def make_bitmap(image_path):
    image = resize_image(image_path)
    column = []
    for x in range(OLED_HEIGHT):
        row = []
        for y in range(OLED_WIDTH):
            pixel = image.getpixel((y, x)) // 255
            row.append(pixel)
        column.append(row)
    return column

def make_bitmap_string(bitmap):
    bitmap_string = f"#ifndef SPLASH\n#define LOGO_WIDTH {OLED_WIDTH}\n#define LOGO_HEIGHT {OLED_HEIGHT}\n"
    bitmap_string += "const uint8_t PROGMEM logo_bmp[] = {\n"
    for row in bitmap:
        bitmap_string += "\t"
        for index, pixel in enumerate(row):
            if index == 0:
                bitmap_string += "0b"
            elif index % 8 == 0:
                bitmap_string += ",0b"
            bitmap_string += f"{pixel}"

        bitmap_string += ",\n"
    bitmap_string += "};\n#endif"
    return bitmap_string

def export_bitmap(bitmap_string):
    with open("./splash.h", "w") as file:
        file.write(bitmap_string)

def update_threshold(value):
    # Update the THRESHOLD value and update the displayed image
    global THRESHOLD
    THRESHOLD = threshold_var.get()
    if image_path:
        bitmap = make_bitmap(image_path)
        bitmap_string = make_bitmap_string(bitmap)
        display_image(image_path)
        export_bitmap(bitmap_string)

def open_image():
    global image_path
    image_path = filedialog.askopenfilename(
        filetypes=[("Image files", "*.png *.jpg *.bmp")]
    )
    if image_path:
        bitmap = make_bitmap(image_path)
        bitmap_string = make_bitmap_string(bitmap)
        display_image(image_path)
        export_bitmap(bitmap_string)

def display_image(image_path):
    image = Image.open(image_path)
    image = resize_image(image_path)  # Resize for display
    img = ImageTk.PhotoImage(image)
    image_label.config(image=img)
    image_label.image = img

app = tk.Tk()
app.title("Image to Arduino Bitmap Converter")

# Create a slider for THRESHOLD
threshold_var = tk.IntVar(value=THRESHOLD)
threshold_slider = tk.Scale(app, from_=0, to=255, orient="horizontal", label="Threshold", variable=threshold_var, command=update_threshold)
threshold_slider.pack()

open_button = tk.Button(app, text="Open Image", command=open_image)
open_button.pack()

image_label = tk.Label(app)
image_label.pack()

app.mainloop()
