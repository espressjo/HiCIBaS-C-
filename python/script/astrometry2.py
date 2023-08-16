#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Aug 11 14:18:01 2023

@author: hicibas-clone
"""

from guideCam import coarseCam
from arguments import get_arg_int
from sys import argv
max_stars = get_arg_int("--max-stars",default=-1)
if '--debug' in argv:
    debug = True
else:
    debug = False
if max_stars==-1:
    max_stars=15
with coarseCam() as cam:
    #A = cam.astrometry(max_star=max_stars,sim_image="/home/hicibas-clone/test-loop40.fits")#should update the astrometry telemetry
    A = cam.astrometry(max_star=max_stars,debug=debug)#should update the astrometry telemetry
    print(f"RA: {A.RA}, DEC: {A.DEC}")