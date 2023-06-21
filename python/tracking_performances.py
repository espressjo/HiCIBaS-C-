#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jun 20 10:13:30 2023

@author: espressjo
"""
from natsort import natsorted 
from os import listdir
from os.path import join
from cv2 import moments
from astropy.io import fits
from datetime import datetime 
from matplotlib import pyplot as plt
import seaborn as sns
import numpy as np
sns.set_theme()
fmt ="%Y-%m-%dT%H:%M:%S.%f" 
def get_moment(im,threshold=200):
    im[im<200]=0
    M = moments(im)
    cX = int(M["m10"] / M["m00"])
    cY = int(M["m01"] / M["m00"])
    return cX,cY

def extract(p="/home/hicibas-clone/data"):
    X_guided = [];
    Y_guided = [];
    X_unguided = [];
    Y_unguided = [];
    dt_unguided = [];
    dt_guided = [];
    ls_guided = natsorted( [ join(p,f) for f in listdir(p) if all( ['unguided' not in f, 'guided' in f,'fits' in f] ) ] )
    ls_unguided = natsorted( [ join(p,f) for f in listdir(p) if all([ 'unguided' in f,'fits' in f]) ] )
    for f in ls_guided:
        x,y = get_moment(fits.getdata(f))
        dt_guided.append(  datetime.strptime(fits.getheader(f)['DATE'],fmt)  )
        X_guided.append(x)
        Y_guided.append(y)
    for f in ls_unguided:
        x,y = get_moment(fits.getdata(f))
        X_unguided.append(x)
        Y_unguided.append(y)
        dt_unguided.append(  datetime.strptime(fits.getheader(f)['DATE'],fmt)  )
    dt_unguided = np.asarray(dt_unguided)
    dt_unguided-=dt_unguided[0]
    dt_guided = np.asarray(dt_guided)
    dt_guided-=dt_guided[0]
    t_unguided = [t.total_seconds() for t in dt_unguided]
    t_guided = [t.total_seconds() for t in dt_guided]
    return zip(t_guided,X_guided,Y_guided),zip(t_unguided,X_unguided,Y_unguided)
def plot(data,title):
    fig,ax = plt.subplots()
    plt.ion()
    ax1 = ax.twinx()
    t,x,y = zip(*data)
    ax.plot(t,x,'ro',alpha=0.5,label="X-Axis")
    ax1.plot(t,y,'go',alpha=0.5,label="Y-Axis")
    ax.set(xlabel="Time (seconds)",ylabel="X-axis (pixels)",title=title)
    ax1.set(ylabel="Y-axis (pixels)")
    ax1.legend()
    ax1.set_ylim((400,600))
    plt.tight_layout()
    return fig,ax,ax1
    
if '__main__' in __name__:
    from system import argv
    if len(argv)==2:
        path = argv[1]
    else:
        path = "/home/hicibas-clone/data"
    guided,unguided = extract("/home/hicibas-clone/data")
    fig_ug,_,_ = plot(unguided,"Unguided")
    fig_g,_,_ = plot(guided,"Guided")
    fig_ug.savefig(join(path,"unguided.png"))
    fig_g.savefig(join(path,"guided.png"))
    plt.show()    
    
