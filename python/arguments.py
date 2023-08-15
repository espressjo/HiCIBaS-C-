#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Aug  3 07:26:53 2023

@author: espressjo
"""

from sys import argv
from os.path import isfile

def get_int(key,file,default=-999):
    if not isfile(file):
        return default 
    with open(file,'r') as f:
        for l in f.readlines():
            if key in l:
                k = l.strip().split(" ")[-1]
                try:
                    return int(k)
                except:
                    return default
    return default
def get_float(key,file,default=-999.999):
    if not isfile(file):
        return default 
    with open(file,'r') as f:
        for l in f.readlines():
            if key in l:
                k = l.strip().split(" ")[-1]
                try:
                    return float(k)
                except:
                    return default
    return default
def get_str(key,file,default=""):
    if not isfile(file):
        return default 
    with open(file,'r') as f:
        for l in f.readlines():
            if key in l:
                k = l.strip().split(" ")[-1]
                try:
                    return k
                except:
                    return default
    return default
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
    return -1
            
def get_arg_float(arg):
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
                return float(a)
            except:
                return -1
    return -1
def get_arg_string(arg):
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
                return a
            except:
                return -1
    return -1