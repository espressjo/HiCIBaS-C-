#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Aug 18 15:03:05 2023


Arguments: 
    --debug: interactive debug
    --max-stars: Max nb. of stars to find for astrometry
    --x:    Where we want to place the target, if not defined we center in fine cam FOV
    --y:    Where we want to place the target, if not defined we center in fine cam FOV
    --ra:   Which target we want to move. In (hour) or HMS
    --dec:   Which target we want to move. In (deg) or HMS
    --dry: Will perform a dry run without moving the telescope
    --fake: Will take a test image and not the real output of coarse cam
    --brightest: We will us DAOfind to center the brightest start instead of astrometry
    --fwhm: Adjust the fwhm for DAOfinder


@author: espressjo
"""
from astropy.io import fits
from photutils.detection import DAOStarFinder
from astropy.stats import sigma_clipped_stats as sc 
from arguments import get_arg_float,get_arg_int,get_arg_string
from sys import exit,argv 
from guideCam import coarseCam  
from os import popen
import astropy.units as u
from astropy.coordinates import SkyCoord
from moteurs2 import moteurs2
from os.path import isfile
ALT = 35.25#steps/pixels
AZ = 51.24999999999999 #theoritical calculation

X = get_arg_int("--X=",default=769)
Y = get_arg_int("--Y=",default=549)


def get_data():
    if '--fake' in argv:
        f = "~/test-loop40.fits"
        im = fits.getdata(f)
        
    else:
        with coarseCam() as cam:
            cam.get_data()
            im = cam.get_data()
    fits.PrimaryHDU(data=im).writeto("/var/tmp/stars.fits",overwrite=True)
    if isfile("/var/tmp/stars.fits.gz"):
        popen("rm -f /var/tmp/stars.fits.gz")
    popen("cd /var/tmp && gzip stars.fits").read()
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
        print("No star found!")
        exit(0)
    sources.sort('peak',reverse=True)
    return sources[0]["xcentroid"],sources[0]["ycentroid"],sources[0]["peak"]


if '__main__' in __name__:
    
    if "--brightest" in argv:
        x,y,peak = find_brightest()    
        y_err = (Y-y)
        x_err = (x-X)
        nutec_x = int(x_err*ALT)
        rm8_y = int(y_err*AZ)
        if "--debug"  in argv or "--dry" in argv:
            print("Brightest: %.1f %.1f"%(x,y))
            print(f"FOV (fine): {X} {Y}")
            print("Will move by -> X: %.1f, Y: %.1f pixels"%(x_err,y_err))
            print("Nutec: %.1f°, Y: %.1f°"%(nutec_x/10000.,rm8_y/10000.))
            exit(0)
            
        with moteurs2() as M:
            if "--dry" not in argv:
                M.move(nutec_x,rm8_y)
        print("Action performed Star [%d,%d] -> [%d,%d]"%(x,y,X,Y))
    else:
        
        max_stars = get_arg_int("--max-stars",default=15)
        
        ra = get_arg_string("--ra")
        dec = get_arg_string("--dec=")
        if ":" in ra:
            C = SkyCoord(ra,dec, unit=(u.hour,u.degree),frame='icrs')
        else:
            C = SkyCoord(float(ra),float(dec), unit=(u.hour,u.degree),frame='icrs')
        with coarseCam() as cam:
            if "--fake" in argv:
                A = cam.astrometry(max_star=max_stars,sim_image="~/test-loop40.fits")
            else:
                A = cam.astrometry(max_star=max_stars)#should update the astrometry telemetry
            if not A.valid:
                print("Astrometry failed")
                exit(0)
        x,y = A.wcs.world_to_pixel_values(C.ra.degree,C.dec.degree)
        y_err = (Y-y)
        x_err = (x-X)
        nutec_x = int(x_err*ALT)
        rm8_y = int(y_err*AZ)
        if "--dry" in argv:
            print(f"Object is here-> x: {x}, Y:{y}")
            print(f"Will move by -> X: {x_err}, Y: {y_err} pixels or,")
            print(f"by -> Nutec: {nutec_x/10000.}°, Y: {rm8_y/10000.}°")
            print("")
        

        with moteurs2() as M:
            if "--dry" not in argv:
                M.move(nutec_x,rm8_y)
        print("Action performed Star [%d,%d] -> [%d,%d]"%(x,y,X,Y))