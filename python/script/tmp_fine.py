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
from guideCam import fineCam as C

tmp = "/var/tmp/tmp.fine.fits"

with C() as cam:
    cam.get_data()#the 1st image is often scrap.
    cam.save(tmp,True)
    
sleep(0.5)

if isfile(tmp):
    popen(f"gzip -f {tmp}").read()

print(f"{tmp}.gz saved!",file=stdout)
exit(0)
