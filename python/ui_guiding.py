#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon May  8 12:08:53 2023

@author: espressjo
"""
SIMULATION = True

from guideCam import guideCam
from sys import argv
from time import sleep

def isInt(arg:str):
    return all([c.isdigit()  for c in arg ])
def isFloat(arg:str):
    return all([ any([c.isdigit(),c=='.',c=="-",c=="+"])   for c in arg ])

def getArg(arg_name:str):
    #must be of the form --arg=110.0
    for arg in argv:
        if arg_name in arg:
            arg = arg.replace(arg_name,"")
            if isInt(arg):
                return int(arg)
            elif isFloat(arg):
                return float(arg)
            else:
                return arg
    return ""

#::::::::::::::::::::::::::::::::::::::::::::::::
#:::   Capture an image and return the info   :::
#::::::::::::::::::::::::::::::::::::::::::::::::
  
def capture_extract():
    with guideCam() as GC:
        if getArg("--set_expt=")!="":
            t = getArg("--set_expt=")
            GC.set_expt(t)
            sleep(1)
        GC.simulation = SIMULATION
        info = GC.get_current_fov()
    print(info)
    return 0


if '__main__' in __name__:
    if '--capture-fov' in argv:
        capture_extract()
