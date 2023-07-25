#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jul 11 08:21:00 2023

@author: espressjo
"""
import os 
from moteurs2 import moteurs2 as mot 
from guideCam import coarseCam
import threading
from time import sleep
from time import perf_counter as pc
from guiding_light import mkdir,IP,LSPEED,HSPEED,ACC,AMP
from natsort import natsorted
from datetime import datetime 
from astropy.io import fits
from simple_pid import PID
import numpy as np

path = "/home/hicibas-clone/data"
#FREQ = 15
ALT = 35.25#steps/pixels
AZ = 51.24999999999999 #theoritical calculation
TIME = 15

def get_last_inc(p):
    ls = natsorted([ f for f in os.listdir(p) if '.fits' in f  ])
    if len(ls)<1:
        return 0
    last = ls[-1]
    inc = last.replace("data_","").replace(".fits","")
    if not inc.isdigit():
        print("Problem with increment!!!!")
        exit(1)
    return int(inc)

def create_new_folder():   
    """
    Create new folder  in ~/data
    
    Return
    ------
        return the newly created folder in ~/data
    """
    ls = natsorted([d for d in os.listdir(path) if all([os.path.isdir(os.path.join(path,d)),d.isdigit()])])
    if len(ls)>0:
        last = ls[-1]
        nlast = "%.3d"%(int(last)+1)
        p = os.path.join(path,nlast)
        mkdir(p)
        return p
    else:
        mkdir(os.path.join(path,"001"))
        return os.path.join(path,"001")
def clear():
    #clear the screen
    os.system("clear")
def save(fname,data,freq,guiding=True,pid=None):
    """
    Save data, fname must include the path

    """
    t = datetime.now().strftime("%Y-%m-%dT%H:%M:%S.%f" )
    hdul = fits.HDUList([fits.PrimaryHDU(data=data)])
    hdul[0].header['DATE'] = t
    if pid!=None:
        hdul[0].header['P'] = pid[0]
        hdul[0].header['I'] = pid[1]
        hdul[0].header['D'] = pid[2]
    hdul[0].header['GUIDING'] = guiding
    hdul[0].header['FREQ'] = freq
    hdul.writeto(fname)

def acquire_pid(p,X,Y,freq,guiding=True,tolerance=30,pid_v=(1.0,0.15,0)):
    fname = os.path.join(p,"data_%.7d.fits")
    i = get_last_inc(p)    
    with mot() as m:
        with coarseCam() as cam:
            t2 = 0
            t1 = 0
            OK = True
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
                    
                    if abs(int(x_err*ALT))<tolerance:
                        x_err=0
                    
                    t5 = pc()
                    if guiding:    
                        m.move_no_return(int(x_err*ALT),int(y_err*AZ),)  
                    t6 = pc()
                    
                    t7 = pc()
                    save(fname%i,im,freq,guiding=guiding,pid=pid_v)
                    t8 = pc()
                    t2 = pc()
                    clear()
                    print(f'[{datetime.now().strftime("%H:%M:%S.%f")}] {fname%i}')
                    print(f"Move: X_err: {x_err}, Y_err: {y_err}")
                    print(f"steps: X: {int(x_err*ALT)}, Y: {int(y_err*AZ)}")
                    print("[%.5f s] moments"%(t4-t3))
                    print("[%.5f s] motors"%(t6-t5))
                    print("[%.5f s] Saving FITS"%(t8-t7))                    
                else:
                    OK = True
                    sleep(1/freq - (t2-t1))     
                stop = datetime.now()
                if (stop-start).total_seconds()>TIME:
                    L=False
    return
def set_rm8(lspeed,hspeed,acc):
    from rm8 import rm8
    print("setting RM8")
    with rm8("localhost",7565) as rm:
        print(rm.set_high_speed(int(hspeed)))
        print(rm.set_low_speed(int(lspeed)))
        print(rm.set_acceleration(int(acc)))
    input("press any key to continue")
def acquire(p,X,Y,freq,guiding=True,tolerance=30):
    fname = os.path.join(p,"data_%.7d.fits")
    i = get_last_inc(p)
    with mot() as m:
        with coarseCam() as cam:
            t2 = 0
            t1 = 0
            OK = True
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
                    if abs(int(y_err*AZ))<tolerance:
                        y_err = 0
                    if abs(int(x_err*ALT))<tolerance:
                        x_err=0
                    t5 = pc()
                    if guiding:
                        m.move_no_return(int(x_err*ALT),int(y_err*AZ),)  
                    t6 = pc()
                    t7 = pc()
                    save(fname%i,im,freq,guiding=guiding)
                    t8 = pc()
                    t2 = pc()
                    clear()
                    print(f'[{datetime.now().strftime("%H:%M:%S.%f")}] {fname%i}')
                    print(f"Move: X_err: {x_err}, Y_err: {y_err}")
                    print(f"steps: X: {int(x_err*ALT)}, Y: {int(y_err*AZ)}")
                    print("[%.5f s] moments"%(t4-t3))
                    print("[%.5f s] motors"%(t6-t5))
                    print("[%.5f s] Saving FITS"%(t8-t7))
                else:
                    OK = True
                    sleep(1/freq - (t2-t1))     
                stop = datetime.now()
                if (stop-start).total_seconds()>TIME:
                    L=False
    return
from rm8 import rm8
def move_light(loop=-1,delay=-1):
    """
    Move the light install on the RM3. 

    Parameters
    ----------
    loop : INT, optional
        Number of loop. -1 will move the light non-stop. The default is -1.

    Returns
    -------
    None.

    """
    if delay!=-1:
        sleep(delay)
    with rm8(IP,7565) as m:
        m.enable_drive()
        m.set_low_speed(LSPEED*10000)
        m.set_high_speed(HSPEED*10000)
        m.set_acceleration(ACC*10000)
        i=-1
        
        global stop_thread
        stop_thread = False
        while(not stop_thread):
            while(m.isMoving()):
                sleep(0.1)
            i+=1
            print("Moving ...")
            if i%2==0:
                m.move(AMP*10000)
            else:
                m.move(-1*AMP*10000)
            if loop!=-1:
                if loop<i:
                    break
    return
if '__main__' in __name__:
    set_rm8(2e4,4e4,2.3e4)
    light_t = threading.Thread(target=move_light,args=(-1,))
    print("Starting light movement")
    light_t.start()
    sleep(1)
    
    P = create_new_folder()
    acquire(P,500,500,10,guiding=False)
    for f in [5,8,11,14,17,20]:
        acquire(P,500,500,f,guiding=True)
    
    """
    for _P in np.linspace(0.6,1.0,5):
        for _I in [0.01,]:#np.linspace(0.01,0.3,2):
            acquire_pid(P,500,500,guiding=True,pid_v=(_P,_I,0))
    """        
    #stop the light thread
    stop_thread = True
    light_t.join()
    
