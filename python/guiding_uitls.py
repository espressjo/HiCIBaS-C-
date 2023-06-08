#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Jun  7 17:31:33 2023

@author: espressjo
"""
from ds9 import newFits,crossHair,tofit
from guideCam import fineCam,coarseCam
from moteurs import moteurs
from time import sleep

def display(x,y):
    """
    For troubleshooting purpose only. Open a ds9 instance 1st,
    then run this function to update the last tmp.fits file
    taken.

    Parameters
    ----------
    x : TYPE
        DESCRIPTION.
    y : TYPE
        DESCRIPTION.

    Returns
    -------
    None.

    """
    newFits("/var/tmp/tmp.fits")
    sleep(0.1)
    crossHair(x,y)
    sleep(0.1)
    tofit()
    print(f"X: {x}, Y: {y}")

def move_coarse(X,Y,ite=3,timeout=5):
    """
    Move the coarse image moment to a specific pixel X,Y.

    Parameters
    ----------
    X : INT
        X-axis position.
    Y : INT
        Y-axis position.
    ite : INT, optional
        How many iteration. The Nutec motor might take 2-3 iteration 
        before reaching the position. The default is 3.
    timeout : INT, optional
        Timeout for each loop. The default is 5.

    Returns
    -------
    TYPE
        Return the image moment after the movement is performed.

    """
    ratio_alt = 35.25#steps/pixels
    ratio_az = 40.01411839280821#steps/pixels
    
    with coarseCam() as cam:
        with moteurs() as mot:
            for i in range(ite):
                x,y = cam.get_moment()
                x_err = x-X
                y_err = Y-y
                mot.move_steps(int(y_err*ratio_az),int(x_err*ratio_alt))
                sleep(timeout)
        return cam.get_moment()
def move_fine(X,Y,ite=3,timeout=5):
    """
    Move the fine image moment to a specific pixel X,Y.

    Parameters
    ----------
    X : INT
        X-axis position.
    Y : INT
        Y-axis position.
    ite : INT, optional
        How many iteration. The Nutec motor might take 2-3 iteration 
        before reaching the position. The default is 3.
    timeout : INT, optional
        Timeout for each loop. The default is 5.

    Returns
    -------
    TYPE
        Return the image moment after the movement is performed.

    """
    ratio_alt = 1.2# ~ 1.2 pixel per steps 
    ratio_az = 0.8478787878787877#pixels/steps
    with fineCam() as cam:
        with moteurs() as mot:
            for i in range(ite):
                x,y = cam.get_moment()
                y_err = Y-y
                x_err = X-x
                #:::   test   :::
                print(f"will move to Az: {int(y_err*ratio_az)} steps and Alt: {int(x_err*ratio_alt)} steps")
                cam.save("/var/tmp/tmp.fits")
                display(x, y)
                input("press any key to move")
                print("moving...")
                mot.move_steps(int(y_err*ratio_az),int(x_err*ratio_alt))
                print("sleeping...")
                sleep(timeout)
            return cam.get_moment()