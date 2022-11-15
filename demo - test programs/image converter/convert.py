

import PIL
from PIL import Image,ImageDraw,ImageFont
import io
# -*- coding:utf-8 -*-
import sys
import os
import numpy as np

import textwrap


def test(im):
    fil = open('file', 'w')
    pixel = im.load()
    row, column = im.size
    for y in range(column):
        for x in range(row):
            pixel = pixel[x, y]
            fil.write(str(pixel) + '\n')

def getbuffer(width, height, image):
    # Create a pallette with the 7 colors supported by the panel
    pal_image = Image.new("P", (1,1))
    pal_image.putpalette( (0,0,0,  255,255,255,  0,255,0,   0,0,255,  255,0,0,  255,255,0, 255,128,0) + (0,0,0)*249)

    # Check if we need to rotate the image
    imwidth, imheight = image.size
    if(imwidth == width and imheight == height):
        image_temp = image
    elif(imwidth == height and imheight == width):
        image_temp = image.rotate(90, expand=True)
    else:
        print("Invalid image dimensions: %d x %d, expected %d x %d" % (imwidth, imheight, width, height))

    # Convert the soruce image to the 7 colors, dithering if needed
    image_7color = image_temp.convert("RGB").quantize(palette=pal_image)
    image_7color.save("test.bmp")
    #test(image_7color)
    np.savetxt("pixel_data.txt", image_7color, fmt='%d', delimiter=" ")
    buf_7color = bytearray(image_7color.tobytes('raw'))

    # PIL does not support 4 bit color, so pack the 4 bits of color
    # into a single byte to transfer to the panel
    buf = [0x00] * int(width * height / 2)
    idx = 0
    for i in range(0, len(buf_7color), 2):
        buf[idx] = (buf_7color[i] << 4) + buf_7color[i+1]
        idx += 1

    return buf

test_image = Image.open('faultyiconpaint.bmp')
image_buffer = getbuffer(600,448,test_image)
np.savetxt("faultyiconpaint.txt", image_buffer, fmt='%d', delimiter=" ")
hex_array = ',0x'.join('{:02x}'.format(x) for x in image_buffer)
print(hex_array)
#wrapped_hex = textwrap.fill(hex_array, 104)

text_file = open("sample.txt", "w")
n = text_file.write(hex_array)
text_file.close()
print(image_buffer)
