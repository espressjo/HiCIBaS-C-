#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Jul 10 12:47:38 2023

@author: espressjo
"""
from guideCam import coarseCam as cCam
from os import popen,listdir
from os.path import join

fmt = "%Y-%m-%dT%H:%M:%S.%f"
p = "/home/hicibas-clone/data"
print(popen(f"mkdir -p {p}").read())
#p = "/home/hicibas-clone/data"
print(popen(f"mkdir -p {p}").read())
ls = [join(p,f) for f in listdir(p) if "setup_" in f]
for f in ls:
    popen(f"rm {f}").read()



i= 0
with cCam() as cam:
    while(1):
        i+=1
        cam.save(join(p,"setup_%.8d.fits"%i),overwrite=True)
    
    


