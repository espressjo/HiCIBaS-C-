#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Aug 18 15:58:08 2023

@author: espressjo
"""
from astropy.io import fits
from photutils.detection import DAOStarFinder
from astropy.stats import sigma_clipped_stats as sc 
from sys import exit,argv 
from guideCam import coarseCam  
from arguments import get_arg_int,get_arg_float
from time import perf_counter as pc
import numpy as np


def get_data():
    if '--fake' in argv:
        f = "~/test-loop40.fits"
        im = fits.getdata(f)
        
    else:
        with coarseCam() as cam:
            cam.get_data()
            im = cam.get_data()
    
    return im

def find_brightest():
    """
    [1] take an image
    [2] save compress version of image for troubleshooting
    [3] detect stars
    [*] If no stars are found it shutdown the script
    [4] return the brightest star coordinate



    Returns
    -------
    X
        x coordinate.
    Y
        y coordinate.
    PEAK
        Peak value of star.

    """
    im = get_data()
        
    fwhm = get_arg_float("--fwhm=",default=3.0)
    mn,md,std = sc(im,sigma=5)
    daofind = DAOStarFinder(fwhm=fwhm, threshold=(md+5.*std))  
    sources = daofind(im - md)  
    if not sources:
        return np.nan,np.nan
    sources.sort('peak',reverse=True)
    return sources[0]["xcentroid"],sources[0]["ycentroid"]
    


time = get_arg_int("--seconds=",default=30)

X = [];
Y = [];
t1 = pc()

while(1):
    if (pc()-t1)>time:
        break
    data = get_data()   
    x,y = find_brightest()
    if np.isnan(x) or np.isnan(y):
        continue
    X.append(x)
    Y.append(y)
if len(X)==0:
    print("No star found!")
    exit(0)
x_mean = np.mean(X)
y_mean = np.mean(Y)
x_std = np.std(X)
y_std = np.std(Y)

if "--print" in argv:
    i = np.linspace(0,len(X),10,dtype=int)
    for _i in i:
        print("[%d] (x,y) %d, %d"%(_i+1,X[_i],Y[_i]))
    print("------------------------")
print("X: %d+/-%d, Y: %d+/-%d"%(x_mean,x_std,y_mean,y_std))
exit(0)