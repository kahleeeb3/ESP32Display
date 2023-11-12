from tkinter import filedialog
from PIL import Image

byte_data = [] # place to store image byte data

def getImageData(image):
    global byte_data
    imageData = list(image.getdata()) # get data of image [0, 255, 0, ..., 255]
    binaryImageData = [1 if pixel == 255 else 0 for pixel in imageData] # convert values to bit representation
    binaryChunks = [binaryImageData[i:i+8] for i in range(0, len(binaryImageData), 8)] # break into 8 bit chunks
    binaryStrings = [''.join(map(str, chunk)) for chunk in binaryChunks] # convert chunks to strings
    byteValues = [int(binaryString, 2) for binaryString in binaryStrings] # convert each section to an (byte)
    byte_data = byteValues # store the byte data globally

def modifyImage(imagePath):
    image = Image.open(imagePath) # open image
    image = image.resize((128, 64), Image.LANCZOS)  # shrink image with LANCZOS resampling
    image = image.convert("L") # convert to grayscale
    image = image.point(lambda p: p > 115 and 255)  # Apply threshold
    image = image.convert("1")  # Convert to monochrome
    return image

def openImage():
    imagePath = filedialog.askopenfilename(
        filetypes=[("Image files", "*.png *.jpg")]
    )
    if imagePath:
        image = modifyImage(imagePath) # make the image work with display
        getImageData(image)
        return image