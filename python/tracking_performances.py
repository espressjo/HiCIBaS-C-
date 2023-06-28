#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jun 20 10:13:30 2023

@author: espressjo
"""
from astropy.stats import sigma_clipped_stats as sc
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
ratio_az = 51.0
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
    rest = {};
    ls_guided = natsorted( [ join(p,f) for f in listdir(p) if all( ['unguided' not in f, 'guided' in f,'fits' in f] ) ] )
    ls_unguided = natsorted( [ join(p,f) for f in listdir(p) if all([ 'unguided' in f,'fits' in f]) ] )
    for f in ls_guided:
        x,y = get_moment(fits.getdata(f))
        dt_guided.append(  datetime.strptime(fits.getheader(f)['DATE'],fmt)  )
        H = fits.getheader(f)
        if 'P' in H:
            p = fits.H(f)['P']
            i = fits.H(f)['I']
            d = fits.H(f)['D']
            name = "%0.3f-%0.3f-%0.3f"%(p,i,d)
            if name not in d:
                rest[name] = [];
            rest[name].append(y)
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
    return zip(t_guided,X_guided,Y_guided),zip(t_unguided,X_unguided,Y_unguided),rest
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
    mn,_,std = sc(y)
    ax1.set_ylim((mn-3*std,mn+3*std))
    plt.tight_layout()
    return fig,ax,ax1
def plot_y(t1,y1,t2,y2,pid=None,title="Y-axis Comparaison"):
    fig,ax = plt.subplots()
    _,_,std = sc(y1)
    ax.plot(t1,y1,alpha=0.5,label="Guided %d arcsec"%(std*0.32*ratio_az))
    _,_,std = sc(y2)
    ax.plot(t2,y2,alpha=0.5,label="Unguided %d arcsec"%(std*0.32*ratio_az))
    ax.set(xlabel="Time (seconds)",ylabel="Y-axis (pixels)",title=title)
    ax.legend()
    plt.tight_layout()
    return fig
if '__main__' in __name__:
    from sys import argv
    if len(argv)==2:
        path = argv[1]
    else:
        path = "/home/hicibas-clone/data"
    guided,unguided,pid = extract(path)
    t1,_,y1 = zip(*guided)
    t2,_,y2 = zip(*unguided)
    fig = plot_y(t1,y1,t2,y2,title="No PID")
    for name in pid:
        y1 = pid[name]
        plot_y(t1,y1,t2,y2,title=name)
    plt.show()    
    input("any key")
