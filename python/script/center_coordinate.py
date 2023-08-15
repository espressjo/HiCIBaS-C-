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
    --x:    Where we want to place the target
    --y:    Where we want to place the target
    --ra:   Which target we want to move. In (hour) or HMS
    --dec:   Which target we want to move. In (deg) or HMS

"""



if "--debug" in argv:
    debug = True
else:
    debug = False

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
if max_stars==-1:
    max_stars=15
with coarseCam() as cam:

    A = cam.astrometry(max_star=max_stars)#should update the astrometry telemetry
    if not A.valid:
        print("Astrometry failed")
        exit(0)
    popen("mv /var/tmp/.astrom/process.new ~/t1.fits").read()
    x,y = A.wcs.world_to_pixel_values(C.ra.degree,C.dec.degree)
    if debug:
        print(f"Object is here-> x: {x}, Y:{y}")
    y_err = (guiding_y-y)
    x_err = (x-guiding_x)
    if debug:
        print(f"Will move by -> x_err: {x_err}, Y_err: {y_err} pixels")
    nutec_x = int(x_err*ALT)
    rm8_y = int(y_err*AZ)
    if debug:
        input("any key to move")
    with moteurs2() as M:
        M.move(nutec_x,rm8_y)
    sleep(3)
