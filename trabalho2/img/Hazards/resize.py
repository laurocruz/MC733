#!/bin/python3
from PIL import Image
from resizeimage import resizeimage
import glob

for filename in glob.iglob('./*.jpg'):
    with open(filename, 'r+b') as f:
        if "small" not in filename:
            print(filename)
            filename = filename.replace(".jpg",'')
            with Image.open(f) as image:
                cover = resizeimage.resize_cover(image, [400, 250])
                cover.save(filename+".small.jpg", image.format)

for filename in glob.iglob('./*.png'):
    with open(filename, 'r+b') as f:
        if "small" not in filename:
            print(filename)
            filename = filename.replace(".png",'')
            with Image.open(f) as image:
                cover = resizeimage.resize_cover(image, [400, 250])
                cover.save(filename+".small.png", image.format)
