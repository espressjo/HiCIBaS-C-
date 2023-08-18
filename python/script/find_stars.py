#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Aug 18 15:03:05 2023

@author: espressjo
"""
from astropy.io import fits
from photutils.detection import DAOStarFinder
from astropy.stats import sigma_clipped_stats as sc 
from arguments import get_arg_float
from sys import exit,argv 
from guideCam import coarseCam  
from os import popen
from os.path import isfile



def find(im):
    fwhm = get_arg_float("--fwhm=",default=3.0)
    mn,md,std = sc(im,sigma=5)
    daofind = DAOStarFinder(fwhm=fwhm, threshold=(md+5.*std))  
    sources = daofind(im - md)  
    if not sources:
        return sources
    sources.sort('peak',reverse=True)
    return sources

with coarseCam() as cam:
    cam.get_data()
    im = cam.get_data()


fits.PrimaryHDU(data=im).writeto("/var/tmp/stars.fits",overwrite=True)
if isfile("/var/tmp/stars.fits.gz"):
    popen("rm -f /var/tmp/stars.fits.gz")
popen("cd /var/tmp && gzip stars.fits").read()

if '--fake' in argv:
    f = "~/test-loop40.fits"
    im = fits.getdata(f)
    
sources = find(im)
 
if not sources:
    print("No source found!")
    print("/var/tmp/stars.fits.gz saved")
    exit(0)

if '--verbose' in argv:
    for s in sources:
        print("[%d ADU]  X: %.1f, Y: %.1f"%(s['peak'],s['xcentroid'],s['ycentroid']))
    print("------------------------------")

print("[%d ADU]  X: %.1f, Y: %.1f"%(sources[0]['peak'],sources[0]['xcentroid'],sources[0]['ycentroid']))
exit(0)

