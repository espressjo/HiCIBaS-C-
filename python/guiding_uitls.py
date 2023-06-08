#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Jun  7 17:31:33 2023

@author: espressjo
"""
from ds9 import *
from guideCam import fineCam,coarseCam
from moteurs import moteurs
from time import sleep
def display(x,y):
    newFits("/var/tmp/tmp.fits")
    sleep(0.1)
    crossHair(x,y)
    tofit()
    print(f"X: {x}, Y: {y}")
def move_coarse(X,Y,ite=3,timeout=5):
    ratio_alt = 35.25#steps/pixels
    ratio_az = 40.01411839280821#steps/pixels
    with coarseCam() as cam:
        with moteurs() as mot:
            print("starting!")
            for i in range(ite):
                x,y = cam.get_moment()
                cam.save("/var/tmp/tmp.fits")
                display(x,y)
                x_err = x-X
                y_err = Y-y
                mot.move_steps(int(y_err*ratio_az),int(x_err*ratio_alt))
                input("press any key")
                sleep(timeout)
        return cam.get_moment()