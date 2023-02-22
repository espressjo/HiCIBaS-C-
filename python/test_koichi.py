#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Feb 22 09:55:04 2023

@author: espressjo
"""
from datetime import datetime 
from os import mkdir,listdir 
from os.path import isdir,join
from IDS import ids
from HiCIBaS_CONF import GUIDING_CAM_SERIAL
from natsort import natsorted
from astropy.io import fits
import subprocess
from tqdm import tqdm
from sys import argv
from os import getcwd
fold = datetime.now().strftime("%Y%m%d")
myfits = datetime.now().strftime("%Y%m%d")
myfits+="_%.5d.fits"
P = "C:/Users/Labo/Documents/Data"

if not isdir(P):
    mkdir(P)
P = join(P,fold)
if not isdir(P):
    mkdir(P)
def get_next():
    ls = natsorted([f for f in listdir(P) if '.fits' in f])
    if len(ls)<1:
        return 1
    lfile = ls[-1]
    try:
        i = int(lfile.split('_')[1].replace('.fits',''))
    except:
        print("Something is wrong")
        exit(0)
    return int(i)+1
def ds9(fname):
    subprocess.run(["C:\\SAOImageDS9\\ds9.exe","-zscale",fname])    

exp = [10,30,50,70,90,110]
gain = [1,20,40,60,80]
boost = [True,False]
if '__main__' in __name__:
    if "--show" in argv:
        with ids(GUIDING_CAM_SERIAL) as cam:
            cam.set_adc(12)
            cam.set_memory()
            cam.set_abs_expt(100)
            p = getcwd()
            cam.get_fits(join(p,"test.fits"))
            ds9(join(p,"test.fits"))
        exit(0)
    if '--flat' in argv:
        with ids(GUIDING_CAM_SERIAL) as cam:
            cam.set_adc(12)
            cam.set_memory()
            cam.set_abs_expt(100)
            if 'Y' not in input("We will now acquire 11 images. You will move the camera between each images. [Ready Y/N ?]"):
                exit(0)
            for i in range(11):
                increment = get_next()
                ff = join(P,myfits%increment)
                cam.get_fits(ff)
                data = fits.getdata(ff)
                H = fits.getheader(ff)
                H["TYPE"] = ("FLAT","Type of image")
                fits.PrimaryHDU(data=data,header=H).writeto(ff,overwrite=True)
                input("Move the camera. Press any key to continue")
            exit(0)
    with ids(GUIDING_CAM_SERIAL) as cam:
        cam.set_adc(12)
        cam.set_memory()
        for G in gain:
            print("Working on gain %d"%G)
            if G!=1:
                cam.set_gain(G)
            for b in boost:
                if b:
                    cam.set_gain_boost()
                else:
                    cam.unset_gain_boost()
                for e in tqdm(exp):
                    cam.set_abs_expt(e)
                    increment = get_next()
                    ff = join(P,myfits%increment)
                    cam.get_fits(ff)
                    data = fits.getdata(ff)
                    H = fits.getheader(ff)
                    H["TYPE"] = ("SCIENCE","Type of image")
                    fits.PrimaryHDU(data=data,header=H).writeto(ff,overwrite=True)