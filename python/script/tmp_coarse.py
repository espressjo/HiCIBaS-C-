#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Aug  3 11:41:21 2023

@author: espressjo
"""
from time import sleep
from os import popen
from os.path import isfile
from sys import stdout,exit
from guideCam import coarseCam as C

tmp = "/var/tmp/tmp.fits"

with C() as cam:
    cam.save(tmp,True)
    
sleep(0.5)

if isfile(tmp):
    popen(f"gzip -f {tmp}").read()

print(f"{tmp}.gz saved!",file=stdout)
exit(0)
