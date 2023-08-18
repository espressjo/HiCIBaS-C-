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

fwhm = 3.0


def find(im):
    if get_arg_float("--fwhm=",default=-1)!=-1:
        fwhm = get_arg_float("--fwhm=")
    mn,md,std = sc(im,sigma=5)
    daofind = DAOStarFinder(fwhm=fwhm, threshold=(md+5.*std))  
    sources = daofind(im - md)  
    sources.sort('peak',reverse=True)



if '--fake' in argv:
    f = "~/test-loop40.fits"
with coarseCam() as cam:
    cam.get_data()
    im = cam.get_data()


fits.PrimaryHDU(data=im).writeto("/var/tmp/stars.fits",overwrite=True)
popen("cd /var/tmp && gzip stars.fits").read()

if '--fake' in argv:
    f = "~/test-loop40.fits"
    im = fits.getdata(f)
    
sources = find(im)
 
if len(sources)==0:
    print("No source found!")
    print("/var/tmp/stars.fits.gz saved")
    exit(0)

if '--verbose' in argv:
    for s in sources:
        print(f"Peak: {s['peak']} X: {s['xcentroid']}, Y: {s['ycentroid']}")

print(f"Peak: {sources[0]['peak']}, X: {sources[0]['xcentroid']}, Y: {sources[0]['ycentroid']}")
exit(0)

