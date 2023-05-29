#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed May 24 11:08:33 2023

@author: espressjo
"""
#"/opt/HiCIBaS/config/network.conf"

from os.path import isfile



def get_string(fname,key,default="NULL"):
    if isfile(fname):
        with open(fname,'r') as ff:
            for l in ff.readlines():
                if l.split(' ')[0]==key:
                    l = l.replace(key,'')
                    if '#' in l:
                        l = l.split('#')[0]
                    l = l.replace(' ','')
                    return l.strip() 
    
    return default

def get_int(fname,key,default=-1):
    if isfile(fname):
        with open(fname,'r') as ff:
            for l in ff.readlines():
                if key in l:
                    l = l.replace(key,'')
                    if '#' in l:
                        l = l.split('#')[0]
                    l = l.replace(' ','')
                    try:
                        myInt = int(l)
                        return myInt
                    except:
                        return default
    
    return default

def get_float(fname,key,default=-999.9):
    if isfile(fname):
        with open(fname,'r') as ff:
            for l in ff.readlines():
                if key in l:
                    l = l.replace(key,'')
                    if '#' in l:
                        l = l.split('#')[0]
                    l = l.replace(' ','')
                    try:
                        myFloat = float(l)
                        return myFloat
                    except:
                        return default
    
    return default

if '__main__' in __name__:
    p = get_int("/opt/HiCIBaS/config/network.conf","LJSERIAL")
    print(p)