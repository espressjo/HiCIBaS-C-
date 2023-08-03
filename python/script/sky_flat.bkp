#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Feb 10 15:31:53 2023

@author: espressjo
"""

from guideCam2 import guideCam 
from moteurs2 import moteurs
from code_labjack import Labjack_lim
from sys import stderr
from astropy.io import fits 
import numpy as np
from os.path import join

calpath = "/opt/HiCIBaS/cal"
pos = [(1,0),(0,1),(-1,0),(-1,0),(0,-1),(0,-1),(1,0),(1,0),(-0.5,0.5),(0,-0.5),(-0.5,1.0)]
allFlats = [];

def sky_flat():
    """
    This function will take N images each at a different position.
    

    Returns
    -------
    allFlats : LIST of STR
        DESCRIPTION.

    """
    with moteurs() as m:
        with guideCam() as gc:
            i=1
            for p in pos:
                x,y = p
                fname = gc.get_fits(join(calpath,"flat_%.2d.fits"%(i)))
                allFlats.append(fname)
                i+=1
                m.move(x, y,wait=True)
    return allFlats
def create_flat(ls):
    """
    Stack a list of flats together.

    Parameters
    ----------
    ls : STR LIST
        List of sky flat including absolute path

    Returns
    -------
    STR
        Master Sky flat file.

    """
    cube = [];
    for f in ls:
        data = fits.getdata(f)
        cube.append(data.astype(float))
    flat = np.median(cube,axis=0)
    H = fits.getheader(data)
    fits.PrimaryHDU(data=flat,header=H).writeto(join(calpath,"cal_sky_flat.fits"),overwrite=True)
    return join(calpath,"cal_sky_flat.fits")
if '__main__' in __name__:
    with Labjack_lim(connection="Ethernet") as lim:
        h,zero = lim.anySwitchActivated()
        if h:
            print("Some lim. switch are activated",file=stderr)
            exit(0)
        lim_switch,return_code = lim.execute_with_limswitch(sky_flat)
        if lim_switch!="":
            exit(0)
        else:
            print("%s lim. switch activated in the process",file=stderr)
            exit(1)