import tkinter as tk
# from tkinter import filedialog
# from PIL import Image, ImageTk
from PIL import ImageTk
from photo import openImage
from server import sendData

def displayImage(image, imageLabel):
    img = ImageTk.PhotoImage(image)
    imageLabel.config(image=img)
    imageLabel.image = img

def test():
    from photo import byte_data # get the image data
    print(byte_data)

def submitString(entry):
    user_input = entry.get()
    sendData(user_input)

def createWindow():
    # window
    window = tk.Tk() # define a new window
    window.title('Image Selector') # name the application

    # image
    imageLabel= tk.Label(window)
    imageLabel.pack()
  
    # photo button
    open_button = tk.Button(window, text="Open Image", command=lambda: displayImage(openImage(), imageLabel)) # create button to run openImage
    open_button.pack() # button display mode

    # send data button
    send_button = tk.Button(window, text="Send Data", command=test) # create button to run openImage
    send_button.pack() # button display mode

    # Entry widget for user input
    entry_label = tk.Label(window, text="Enter String:")
    entry_label.pack()

    entry = tk.Entry(window)
    entry.pack()

    # Submit String button
    submit_button = tk.Button(window, text="Submit", command=lambda: submitString(entry))
    submit_button.pack()

    window.mainloop() # run the application

if __name__ == "__main__":
    createWindow()