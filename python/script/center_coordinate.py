#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Aug 11 23:41:09 2023

@author: hicibas-clone
"""
import astropy.units as u
from guideCam import coarseCam
from sys import argv
from moteurs2 import moteurs2
from astropy.coordinates import SkyCoord
from arguments import get_arg_int,get_arg_float,get_arg_string
from time import sleep
from os import popen

"""
Arguments: 
    --debug: interactive debug
    --max-stars: Max nb. of stars to find for astrometry
    --x:    Where we want to place the target, if not defined we center in fine cam FOV
    --y:    Where we want to place the target, if not defined we center in fine cam FOV
    --ra:   Which target we want to move. In (hour) or HMS
    --dec:   Which target we want to move. In (deg) or HMS
    --dry-run: Will perform a dry run without moving the telescope

"""



if "--debug" in argv:
    debug = True
else:
    debug = False
if "--dry-run" in argv:
    dry = True 
else:
    dry =False
ALT = 35.25#steps/pixels
AZ = 51.24999999999999 #theoritical calculation
max_stars = get_arg_int("--max-stars")
ra = get_arg_string("--ra")

dec = get_arg_string("--dec=")
if ":" in ra:
    C = SkyCoord(ra,dec, unit=(u.hour,u.degree),frame='icrs')
else:
    C = SkyCoord(float(ra),float(dec), unit=(u.hour,u.degree),frame='icrs')
guiding_x = get_arg_int("--x=")
guiding_y = get_arg_int("--y=")
#if not specified, we will center in the middle of the FOV of fine CAM
if guiding_x==-999:
    guiding_x = 769
if guiding_y==-999:
    guiding_y = 549
if max_stars==-1:
    max_stars=15
with coarseCam() as cam:

    A = cam.astrometry(max_star=max_stars)#should update the astrometry telemetry
    if not A.valid:
        print("Astrometry failed")
        exit(0)
    
    x,y = A.wcs.world_to_pixel_values(C.ra.degree,C.dec.degree)
    if debug or dry:
        print(f"Object is here-> x: {x}, Y:{y}")
    y_err = (guiding_y-y)
    x_err = (x-guiding_x)
    nutec_x = int(x_err*ALT)
    rm8_y = int(y_err*AZ)
    if debug or dry:
        print(f"Will move by -> X: {x_err}, Y: {y_err} pixels or,")
        print(f"by -> Nutec: {nutec_x/10000.}°, Y: {rm8_y/10000.}°")
    
    if debug:
        input("any key to move")
    with moteurs2() as M:
        if dry:
            M.move(nutec_x,rm8_y)
    sleep(3)
