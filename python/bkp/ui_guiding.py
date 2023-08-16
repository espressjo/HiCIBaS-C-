#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon May  8 12:08:53 2023

@author: espressjo
"""
SIMULATION = True

from sys import argv
from time import sleep
from shm_HiCIBaS import telescope
tel = telescope()
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
import numpy as np
def _extract_info(info:str):
    """
    Used to convert from serialized image info to uint16 values

    Parameters
    ----------
    info : str
        Should have the form --->  Mean;Std;h;w;peak1-x1-y1:peak2-x2-y2:...:peakn-xn-yn

    Returns
    -------
    uint16 arrays
        mean,std, height, width, peaks (arrays),x (arrays),y (arrays)

    """
    mean,std,h,w,pos = info.split(";")
    s = [p.split('-') for p in pos.split(":")]
    if (len(s)>1):
        peak,x,y = zip(*s)
    else:
        peak = [];
        x = [];
        y = [];
    return np.asarray(mean,dtype=np.uint16),np.asarray(std,dtype=np.uint16),np.asarray(h,dtype=np.uint16),np.asarray(w,dtype=np.uint16),np.asarray(peak,dtype=np.uint16),np.asarray(x,dtype=np.uint16),np.asarray(y,dtype=np.uint16)
from astropy.modeling.models import Gaussian2D
from astropy.io import fits
from os import popen

def fake(info,noise=True):
    '''
    From the output of extract_stars you can recreate an image from only the centroid and stats
    value.

    Parameters
    ----------
    info : STR
        See extract_stars for more information.
    noise : BOOL, optional
        Generate background noise based on the real image std. dev. The default is True.

    Returns
    -------
    FOV : 2d numpy array
        Fake FOV.

    '''
    mean,std,h,w,peak,x,y = _extract_info(info)
    if noise:
        rng = np.random.normal(0,std,(h,w))+mean
    else:
        rng = np.zeros((h,w))
    _y, _x = np.mgrid[0:h, 0:w]
    
    for i in range(len(x)):
        rng+=Gaussian2D(peak[i], x[i], y[i], 1.8, 1.8)(_x, _y)
    fits.PrimaryHDU(data=rng).writeto("/var/tmp/data_tmp.fits",overwrite=True)
    popen("xpaset -p ds9 fits /var/tmp/data_tmp.fits").read()
    sleep(0.3)
    popen("xpaset -p ds9 scale zscale").read()
    sleep(0.3)
    popen("xpaset -p ds9 zoom to fit").read()
    return rng
#::::::::::::::::::::::::::::::::::::::::::::::::
#:::   Capture an image and return the info   :::
#::::::::::::::::::::::::::::::::::::::::::::::::

def capture_extract():
    from guideCam import guideCam
    with guideCam(hw_simul=SIMULATION) as GC:
        if not SIMULATION:
            if getArg("--set_expt=")!="":
                t = getArg("--set_expt=")
                GC.set_expt(t)
                sleep(1)
        info = GC.get_current_fov()
    print(info)
    return 0
#::::::::::::::::::::::::::::::::
#:::   Display remote data   ::::
#::::::::::::::::::::::::::::::::
def display():
    if getArg("--data=")!="":
        A = getArg("--data=")
        print("A: "+A)
        fake(A)
    else:
        A = "merde"
    with open("/var/tmp/test.txt","w") as f:
        f.write(A)
if '__main__' in __name__:
    if '--capture-fov' in argv:
        capture_extract()
    if '--display' in argv:
        display()
