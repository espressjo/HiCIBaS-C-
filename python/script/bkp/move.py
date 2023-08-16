#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Aug  2 14:23:04 2023

@author: espressjo
"""
from sys import argv,exit,stdout
from moteurs2 import moteurs2
from guideCam import coarseCam
from time import sleep
ALT = 35.25#steps/pixels
AZ = 51.24999999999999 #theoritical calculation

def get_arg_int(arg):
    """
    extract arguments from system.
    Argument of the form is --arg=xx

    Parameters
    ----------
    arg : TYPE
        DESCRIPTION.

    Returns
    -------
    TYPE
        DESCRIPTION.

    """
    if '--' not in arg:
        arg = '--'+arg
    for a in argv:
        if arg in a:
            a = a.replace(arg,"")
            if "=" in a:
                a = a.replace('=','')
            if '--' in a:
                a = a.replace('--','')
            try:
                return int(a)
            except:
                return -1

def center_brightest():
    """
    Center brigtest star in position x and y.
    Must specify arguement --target-x= and --target-y=

    Returns
    -------
    None.

    """
    X = get_arg_int("target-x")
    Y = get_arg_int("target-y")
    if X==-1 or Y==-1:
        exit(1)
    with coarseCam() as cam:
        with moteurs2() as mot:
            for i in range(3):
                x,y = cam.get_moment()
                x_err = x-X
                y_err = Y-y
                x_nutec=0
                y_rm8=0
                if abs(y_err)>10:
                    x_nutec = ALT*x_err
                if abs(x_err)>10:
                    y_rm8 = AZ*y_err
                mot.move(x_nutec, y_rm8,wait=True)
                sleep(1)
            return cam.get_moment()
                
if '__main__' in __name__:
    if '--center-brightest' in argv:
        x,y = center_brightest()
        print(f"Brightest object center at x: {x}, y: {y}",file=stdout)
        exit(0)