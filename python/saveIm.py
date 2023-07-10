#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Jul 10 12:47:38 2023

@author: espressjo
"""
from guideCam import coarseCam as cCam
from os import popen


fmt = "%Y-%m-%dT%H:%M:%S.%f"
p = "/home/hicibas2/data"
print(popen(f"mkdir -p {p}").read())



i= 0
with cCam() as cam:
    i+=1
    cam.save("setup_%.8d.fits"%i,overwrite=True)
    
    


