#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Jul 10 12:08:00 2023

@author: espressjo
"""
import threading
from rm8 import rm8
from time import sleep
from guideCam import coarseCam as cCam
from os import popen,listdir
from os.path import join
from natsort import natsorted
from astropy.io import fits
from cv2 import moments
from datetime import datetime
fmt = "%Y-%m-%dT%H:%M:%S.%f"
p = "/home/hicibas2/data"
print(popen(f"mkdir -p {p}").read())

def get_moment(im,threshold=200):
    im[im<200]=0
    M = moments(im)
    cX = int(M["m10"] / M["m00"])
    cY = int(M["m01"] / M["m00"])
    return cX,cY


def track():
    i= 0
    with cCam() as cam:
        i+=1
        cam.save("setup_%.8d.fits"%i,overwrite=True)

cam_t = threading.Thread(target=track)        

with rm8("localhost",7565) as m:
    m.enable_drive()
    m.set_low_speed(15*10000)
    m.set_high_speed(50*10000)
    m.set_acceleration(20*10000)
    with cCam() as cam:
        cam_t.start()
        for i in range(4):
            while(m.isMoving()):
                sleep(0.1)
            print("Moving ...")
            if i%2==0:
                m.move(45*10000)
            else:
                m.move(-45*10000)
        cam_t.raise_exception()
        cam_t.join()
            
ls = natsorted([join(p,f) for f in listdir(p) if '.fits' in f])
X =[];
Y = []; 
time = [];
for f in ls:
    data = fits.getdata(f)
    x,y = get_moment(data)
    X.append(x)
    Y.append(y)
    H = fits.getheader(f)
    dt = datetime().strptime(H["DATE"],fmt)
    time.append(dt)
time = [(_dt-dt[0]).total_seconds() for _dt in time]

with open("x-y.csv","a") as f:
    f.write("T,X,Y\n")
    for i in range(len(X)):
        f.write(f"{time[i],X[i]},{Y[i]}\n")
        
from matplotlib import pyplot as plt
import seaborn as sns
sns.set_theme()

fig,ax = plt.subplots()
ax.plot(time,X,'o',label="X")
ax.plot(time,Y,'o',label="Y")
ax.legend()
ax.set(xlabel="Time (s)",ylabel="Position (pixels)",title="Light Tunning")
plt.tight_layout()

    