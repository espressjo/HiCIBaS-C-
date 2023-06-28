#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jun 20 10:39:17 2023

@author: espressjo
"""

from moteurs2 import moteurs2 as mot 
from guideCam import coarseCam
from time import perf_counter as pc
from os import system,popen
from astropy.io import fits 
from datetime import datetime
from time import sleep
from os.path import join,isdir
from simple_pid import PID
from sys import argv
if len(argv)==2:
    path = argv[1]
else:
    path = "/home/hicibas-clone/data"
if not isdir(path):
    print(popen(f"mkdir {path}").read())
freq = 5
ratio_alt = 35.25#steps/pixels
#ratio_az = 40.01411839280821#steps/pixels
ratio_az = 51.24999999999999 #theoritical calculation

def clear():
    system("clear")

def save(fname,data,pid=None):
    t = datetime.now().strftime("%Y-%m-%dT%H:%M:%S.%f" )
    hdul = fits.HDUList([fits.PrimaryHDU(data=data)])
    hdul[0].header['DATE'] = t
    if pid!=None:
        hdul[0].header['P'] = pid[0]
        hdul[0].header['I'] = pid[0]
        hdul[0].header['D'] = pid[0]
    hdul.writeto(fname)


def acquire_pid(fname,path,X,Y,guiding=True,time=60,tolerance=30,pid_v=(1.0,0.015,0)):
    with mot() as m:
        with coarseCam() as cam:
            t2 = 0
            t1 = 0
            OK = True
            i=0
            L = True
            start = datetime.now()
            #test only with the Y axis (Az)
            pid = PID(pid_v[0],pid_v[1],pid_v[2],setpoint=Y)
            pid.sample_time = 1/freq
            while(L):
                if ((t2-t1) >=(1/freq)) or OK:
                    t1=pc()
                    i+=1
                    OK = False
                    t3 = pc()
                    x,y,im = cam.get_moment_data()
                    t4 = pc()
                    x_err = x-X
                    y_err = pid(y)
                    
                    if abs(int(x_err*ratio_alt))<tolerance:
                        x_err=0
                    
                    t5 = pc()
                    if guiding:    
                        m.move_no_return(int(x_err*ratio_alt),int(y_err*ratio_az),)  
                    t6 = pc()
                    f = "%s_%.5d.fits"%(fname,i)
                    t7 = pc()
                    save(join(path,f),im,pid=pid_v)
                    t8 = pc()
                    t2 = pc()
                    clear()
                    print(f'[{datetime.now().strftime("%H:%M:%S.%f")}] {fname}')
                    print(f"Move: X_err: {x_err}, Y_err: {y_err}")
                    print(f"steps: X: {int(x_err*ratio_alt)}, Y: {int(y_err*ratio_az)}")
                    print("[%.5f s] moments"%(t4-t3))
                    print("[%.5f s] motors"%(t6-t5))
                    print("[%.5f s] Saving FITS"%(t8-t7))                    
                else:
                    OK = True
                    sleep(1/freq - (t2-t1))     
                stop = datetime.now()
                if (stop-start).total_seconds()>time:
                    L=False

def acquire(fname,path,X,Y,guiding=True,time=60,tolerance=30):
    with mot() as m:
        with coarseCam() as cam:
            t2 = 0
            t1 = 0
            OK = True
            i=0
            L = True
            start = datetime.now() 
            while(L):
                if ((t2-t1) >=(1/freq)) or OK:
                    t1=pc()
                    i+=1
                    OK = False
                    t3 = pc()
                    x,y,im = cam.get_moment_data()
                    t4 = pc()
                    x_err = x-X
                    y_err = Y-y
                    if abs(int(y_err*ratio_az))<tolerance:
                        y_err = 0
                    if abs(int(x_err*ratio_alt))<tolerance:
                        x_err=0
                    t5 = pc()
                    if guiding:
                        
                        m.move_no_return(int(x_err*ratio_alt),int(y_err*ratio_az),)  
                    t6 = pc()
                    f = "%s_%.5d.fits"%(fname,i)
                    t7 = pc()
                    save(join(path,f),im)
                    t8 = pc()
                    t2 = pc()
                    clear()
                    print(f'[{datetime.now().strftime("%H:%M:%S.%f")}] {fname}')
                    print(f"Move: X_err: {x_err}, Y_err: {y_err}")
                    print(f"steps: X: {int(x_err*ratio_alt)}, Y: {int(y_err*ratio_az)}")
                    print("[%.5f s] moments"%(t4-t3))
                    print("[%.5f s] motors"%(t6-t5))
                    print("[%.5f s] Saving FITS"%(t8-t7))
                else:
                    OK = True
                    sleep(1/freq - (t2-t1))     
                stop = datetime.now()
                if (stop-start).total_seconds()>time:
                    L=False
if '__main__' in __name__:
    print("Staring unguided sequenced")
    sleep(1)
    acquire("unguided",path,500,500,guiding=False,time=20)
    print("Staring guided sequenced")
    sleep(1)
    acquire("guided",path,500,500,guiding=True,time=20)
    for p in range([0.5,0.75,1.0,1.2]):
        acquire_pid("guided",path,500,500,guiding=True,time=20,pid=(p,0.015,0))
