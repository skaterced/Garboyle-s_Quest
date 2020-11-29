#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Nov 29 10:33:01 2020

@author: pi
"""
from PIL.Image import *

print ("Bonjour le monde")

im = open("C:/Users/skate/Documents/programmation Python/LVL_test.png")
#im = open("LVL_0.png")
#im = open("tile_brick.png")

# Image.show(image)
pixels = im.load()

size=24

i=0
print("B", end='')
for y in range(0, size):
    for b in range(0,3):
        for x in range(0,8):
            #print("i",i,"j",j)
            #print (pixels[x,y][0])
            if (pixels[(7-x)+b*8,y]==(255,255,255)):
                print ("1", end='')
            else:
                print("0", end='')
            i+=1
            if (i%8==0):
                print(", ", end='')
                if (i%72==0):
                    print(" ")
                if (i<(24*24)):
                    print("B", end='')                

print ("//Lvl Settings")
print("0x00,")

spike=(255,0,0) #rouge
start=(0,255,255) #cyan
ennemi=(0,0,255) #bleu
coin=(255,255,0) #jaune


for y in range(0, size):
    for x in range(0,size):
        if (pixels[x,y]==(255,0,0)):
            print("BOO!")


#print("0x%x"%(64))
"""

"""