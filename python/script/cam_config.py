#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Aug  3 07:29:45 2023

@author: espressjo
"""


from os.path import join
from sys import argv,exit,stderr
from arguments import get_arg_int,get_arg_float

p = "/opt/HiCIBaS/config"

fine = "fine.cam"
coarse = "coarse.cam"

def update(key,value,file):
    """
    Update the config file

    Parameters
    ----------
    key : STR
        key in the config file
    value : ANY Type
        Value to be uipdated
    file : str
        full path to config filename

    Returns
    -------
    None.

    """
    with open(file, 'r') as f:
        lines = f.readlines()
    with open(file,'w') as ff:
        for l in lines:
            if key in l:
                ff.write(f"{key} {value}\n")
            else:
                ff.write(l)

#::::::::::::::::::::::::::::
#:::   Get the filename   :::
#::::::::::::::::::::::::::::
if '--fine' in argv:
    fname = join(p,"fine.cam")
elif '--coarse' in argv:
    fname = join(p,"coarse.cam")
else :
    print("Missing cam arguments",file=stderr)
    exit(1)

if get_arg_int("--boost=")!=-1:
    update("BOOST",get_arg_int("--boost="),file=fname)
if get_arg_int("--gain=")!=-1:
    update("GAIN",get_arg_int("--gain="),file=fname)
if get_arg_float("--fps=")!=-1:
    update("FPS",get_arg_float("--fps="),file=fname)
if get_arg_float("--exp=")!=-1:
    update("EXP",get_arg_float("--exp="),file=fname)

    