#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Aug 11 23:41:09 2023

@author: hicibas-clone
"""

from guideCam import coarseCam
from sys import argv
from moteurs2 import moteurs2
from astrometry.coordinates import SkyCoord
from arguments import get_arg_int,get_arg_float
from time import sleep

ALT = 35.25#steps/pixels
AZ = 51.24999999999999 #theoritical calculation
max_stars = get_arg_int("--max-stars")
ra = get_arg_float("--ra")
dec = get_arg_float("--dec=")
C = SkyCoord(ra/24 *360,dec,unit="deg")
guiding_x = get_arg_int("--x=")
guiding_y = get_arg_int("--y=")
if max_stars==-1:
    max_stars=15
with coarseCam(hwsimul=True) as cam:
    A = cam.astrometry(max_star=max_stars)#should update the astrometry telemetry
    x,y = A.wcs.world_to_pixel(C)
    y_err = (guiding_y-y)
    x_err = (x-guiding_x)
    nutec_x = int(x_err*ALT)
    rm8_y = int(y_err*AZ)
    with moteurs2() as M:
        M.move(nutec_x,rm8_y)
    sleep(3)