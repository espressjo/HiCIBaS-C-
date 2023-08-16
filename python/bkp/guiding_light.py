#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Jul 10 12:08:00 2023

@author: espressjo
"""
import threading
from rm8 import rm8
from natsort import natsorted
from time import sleep
from cv2 import moments
from astropy.io import fits
from guideCam import coarseCam as cCam
from os import popen,listdir
from os.path import join
from datetime import datetime
from matplotlib import pyplot as plt
import seaborn as sns
sns.set_theme()
import numpy as np

LSPEED = 35
HSPEED = 90
ACC = 30
IP = "10.9.32.206"
AMP = 25

fmt = "%Y-%m-%dT%H:%M:%S.%f"
p = "/home/hicibas-clone/data"
#create the folder if it does not exhist
def mkdir(p):
    print(popen(f"mkdir -p {p}").read())

def get_moment(im,threshold=200):
    im[im<200]=0
    M = moments(im)
    cX = int(M["m10"] / M["m00"])
    cY = int(M["m01"] / M["m00"])
    return cX,cY

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

def redux_light(): 
    """
    Data reduction for the guiding light tuning.

    Returns
    -------
    None.

    """
    path = join(p,"tmp")
    ls = natsorted([join(path,f) for f in listdir(path) if 'setup_' in f])       
    X =[];
    Y = [];
    time = [];
    for f in ls:
        data = fits.getdata(f)
        x,y = get_moment(data)
        X.append(x)
        Y.append(y)
        H = fits.getheader(f)
        print(H["DATE"])
        dt = datetime.strptime(H["DATE"],fmt)
        time.append(dt)
    time = [_dt-time[0] for _dt in time]
    time = [t.total_seconds() for t in time]
    #save data
    with open(join(path,"x-y.csv"),"a") as f:
        f.write("T,X,Y\n")
        for i in range(len(X)):
            f.write(f"{time[i],X[i]},{Y[i]}\n")
    fig,ax = plt.subplots()
    ax1 = ax.twinx()
    ax1.plot(time,Y,'o')
    ax1.set_ylabel("Pixels")
    ax.plot(time,np.asarray(Y)*0.004481907894736842,'o',label="Y")
    ax.legend()
    ax.set(xlabel="Time (s)",ylabel="Position (°)",title="Light Tunning")
    plt.tight_layout()
    fig.savefig(join(path,"x-y.png"))
    plt.show()
    return 

def tune_light(nb_loop=6):
    """
    Will start by moving the light then, it will take images 
    and saved the images in ~/data/tmp

    Returns
    -------
    None.

    """
    path = join(p,"tmp")
    print("Creating tmp path.")
    mkdir(path)
    ls = [join(path,f) for f in listdir(path) if "setup_" in f]
    #erase tmp file
    for f in ls:
        popen(f"rm {f}").read()
    
    light_t = threading.Thread(target=move_light,args=(nb_loop,5))
    light_t.start()
    
    print("Starting Light.")
    sleep(1)
    i= 0
    with cCam() as cam:
        while(light_t.is_alive()):
            i+=1
            cam.save(join(path,"setup_%.8d.fits"%i),overwrite=True)
    return 

                
if '__main__' in __name__:
    from sys import argv
    if '--move' in argv:
        move_light(loop=6)
        exit(0)
    tune_light(nb_loop=6)
    redux_light()
    
