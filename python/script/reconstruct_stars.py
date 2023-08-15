#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Aug 15 16:35:41 2023

@author: espressjo
"""

from mytelnet import myTelnet as tn
from astropy.modeling.models import Gaussian2D
from astropy.io import fits
from os import popen
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
    popen("ds9 -zscale zoom to fit /var/tmp/data_tmp.fits").read()
    
if '__main__' in __name__:
    with tn("172.20.4.140",5555) as r_python:
        out = r_python.write_get_str("python script /opt/HiCIBaS/scripts/extract_stars.py -run")
        print(out)
    if "NULL" in out:
        exit(0)
    fake(out)