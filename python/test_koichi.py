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

fold = datetime.now().strftime("%Y%m%d")
myfits = datetime.now().strftime("%Y%m%d_%.5d.fits")
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
    return int(i)
    

exp = [10,30,50,70,90,110]
gain = [1,20,40,60,80]
boost = [True,False]
if '__main__' in __name__:
    with ids(GUIDING_CAM_SERIAL) as cam:
        cam.set_adc(12)
        cam.set_memory()
        for G in gain:
            if G!=1:
                cam.set_gain(G)
            for b in boost:
                if b:
                    cam.set_gain_boost()
                else:
                    cam.unset_gain_boost()
                for e in exp:
                    cam.set_abs_expt(e)
                    increment = get_next()
                    cam.get_fits(join(P,myfits%increment))