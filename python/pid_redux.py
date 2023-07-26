#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jul 11 09:10:59 2023

@author: espressjo
"""
import os 
from natsort import natsorted
from astropy.stats import sigma_clipped_stats as sc
from os import listdir
from os.path import join,basename
from cv2 import moments
from astropy.io import fits
from datetime import datetime 
from matplotlib import pyplot as plt
import seaborn as sns
import numpy as np
import pandas as pd

sns.set_theme()

AZ = 51.0

fmt ="%Y-%m-%dT%H:%M:%S.%f" 

path = "/home/hicibas-clone/data"

def get_moment(fname,threshold=200):
    hdu = fits.open(fname)
    im = hdu[0].data
    dt = hdu[0].header['DATE']
    im[im<200]=0
    M = moments(im)
    cX = int(M["m10"] / M["m00"])
    cY = int(M["m01"] / M["m00"])
    return datetime.strptime(dt,fmt),cX,cY
def get_last_folder():
    """
    return last folder in ~/data
    """
    ls = natsorted([d for d in os.listdir(path) if all([os.path.isdir(os.path.join(path,d)),d.isdigit()])])
    if len(ls)>0:
        last = ls[-1]
        return os.path.join(path,last)
    else:
        return None

def get_unguided():
    P = get_last_folder()
    ls = natsorted([join(P,f) for f in listdir(P) if '.fits' in f])
    ls = [ f for f in ls if not fits.getheader(f)['GUIDING'] ]
    return natsorted(ls)
def get_guided():
    P = get_last_folder()
    ls = natsorted([join(P,f) for f in listdir(P) if '.fits' in f])
    ls = [ f for f in ls if fits.getheader(f)['GUIDING'] ]
    return natsorted(ls)
def get_guided_pid():
    P = get_last_folder()
    ls = natsorted([join(P,f) for f in listdir(P) if '.fits' in f])
    ls = [ f for f in ls if all([fits.getheader(f)['GUIDING'],'P' in fits.getheader(f)]) ]
    return natsorted(ls)
def split_pid(ls):
    P = [];
    I = [];
    D =0
    di = {};
    for f in ls:
        H = fits.getheader(f)
        P.append(H['P'])
        I.append(H['I'])
    P = set(P)
    I = set(I)
    for p in P:
        for i in I:
            if '%.2f-%.2f-%.2f'%(p,i,D) not in di:
                di['%.2f-%.2f-%.2f'%(p,i,D)] = [];
    for f in ls:
        H = fits.getheader(f)
        h = '%.2f-%.2f-%.2f'%(H['P'],H['I'],0)
        di[h].append(f)
    for h in di:
        di[h] = natsorted(di[h])
    return di
def extract(ls):
    dt = [];
    X  = [];
    Y  = [];
    for f in ls:
        t,x,y = get_moment(f)
        dt.append(t)
        X.append(x)
        Y.append(y)
    dt = [(t-dt[0]).total_seconds() for t in dt]
    return np.asarray(dt),np.asarray(X),np.asarray(Y)
def extract_freq(ls):
    dt = [];
    X  = [];
    Y  = [];
    freq = [];

    for f in ls:
        t,x,y = get_moment(f)
        _freq = fits.getheader(f)["FREQ"]
        dt.append(t)
        X.append(x)
        Y.append(y)
        freq.append(_freq)

    freqs = set(freq)
    D = {};   
    import pdb
    for f in freqs:
         D[str(f)] = [];
         #pdb.set_trace()
         ii = np.where(np.asarray(freq)==f)[0]
         for i in ii:
             D[str(f)].append([dt[i],X[i],Y[i]])
    #pdb.set_trace()
    for n in D:
        start = D[n][0][0]
        for i in range(len(D[n])):
            D[n][i][0] = (D[n][i][0]-start).total_seconds()
    return D
def write(fname,dt,x,y):
    with open(fname,'w') as f:
        f.write("time,x,y\n")
        for i in range(len(dt)):
            f.write("%f,%f,%f\n"%(dt[i],x[i],y[i]))
def get_data(f):
    data = pd.read_csv(f)
    return data['time'].to_numpy(),data['x'].to_numpy(),data['y'].to_numpy()
def tostring(f):
    if '/' in f:
        f = f.split('/')[-1]
    f = f.replace(".csv","")
    p,i,_ = f.split("-")
    return f"P: {p}, I: {i}, D 0"
if '__main__' in __name__:
    from sys import argv
    P = get_last_folder()
    if '--graph-only' not in argv:
        
        #unguided baseline
        ls = get_unguided()
        t,x,y = extract(ls)
        write(join(P,"unguided.csv"), t, x, y)
        
        """
        ls = get_guided()
        v
        write(join(P,"guided.csv"), t, x, y)
        
        ls = get_guided_pid()
        pid = split_pid(ls)
        for h in pid:
            t,x,y = extract(pid[h])
            fname = join(P,h+".csv")
            write(fname, t, x, y)
        """
        #extract unguided with frequency speed
        ls = get_guided()
        D = extract_freq(ls)
        for f in D:
            t,x,y = zip(*D[f])
            fname = join(P,"guided-%sHz.csv"%f)
            write(fname, t, x, y)
    
    #::::::::::::::::::::::::::::::::::::::::
    #:::   guided/unguided Y-axis graph   :::
    #::::::::::::::::::::::::::::::::::::::::
    fig,ax = plt.subplots()
    ax1 = ax.twinx()
    t_ug,x_ug,y_ug = get_data(join(P,"unguided.csv"))
    y_ug = y_ug-np.median(y_ug)
    ax.plot(t_ug,y_ug,alpha=0.5,label="unguided")
    ax.set(title="Performance, std: %.1f pixels"%(np.std(y)),xlabel="Time (second)",ylabel="Amplitude (pixels)")
    ax1.set_ylim(np.asarray(ax.get_ylim())*0.004481907894736842)
    ax1.set_ylabel("Amplitude (°)")
    
    #find all the guided with freq info.
    ls = natsorted([join(P,f) for f in listdir(P) if all(['Hz' in f,'.csv' in f])])
    for fi in ls:
        print(basename(fi))
        t,x,y = get_data(fi)
        freq = int(basename(fi).replace("guided-","").replace("Hz.csv",""))
        y = y-np.median(y)
        std = np.std(y)
        ax.plot(t,y,alpha=0.5,label="%d Hz/%.1f p2p"%(freq,std))
        print("freq [%dHz] ~~> %.1f pixels"%(freq,std))
    
    ax.legend()
    plt.tight_layout()
    fig.savefig(join(P,"guided-y.png"))
    
    #:::::::::::::::::::::::::::::::::::::::
    #:::   guided/unguided graph X-axis  :::
    #:::::::::::::::::::::::::::::::::::::::
    fig,ax = plt.subplots()
    ax1 = ax.twinx()
    t_ug,x_ug,y_ug = get_data(join(P,"unguided.csv"))
    x_ug = x_ug-np.median(x_ug)
    ax.plot(t_ug,x_ug,alpha=0.5,label="unguided")
    ax.set(title="Performance, std: %.1f pixels"%(np.std(x)),xlabel="Time (second)",ylabel="Amplitude (pixels)")
    ax1.set_ylim(np.asarray(ax.get_ylim())*0.004481907894736842)
    ax1.set_ylabel("Amplitude (°)")
    
    #find all the guided with freq info.
    ls = natsorted([join(P,f) for f in listdir(P) if all(['Hz' in f,'.csv' in f])])
    for fi in ls:
        print(basename(fi))
        t,x,y = get_data(fi)
        freq = int(basename(fi).replace("guided-","").replace("Hz.csv",""))
        x = x-np.median(x)
        std = np.std(x)
        ax.plot(t,x,alpha=0.5,label="%d Hz / %.1f px."%(freq,std))
        print("freq [%dHz] ~~> %.1f pixels"%(freq,std))
    
    ax.legend()
    plt.tight_layout()
    fig.savefig(join(P,"guided-x.png"))
    
    """
    t,x,y = get_data(join(P,"guided.csv"))
    y = y-np.median(y)
    ax.plot(t,y,alpha=0.5,label="guided")
    ax.set(title="Performance, std: %.1f pixels"%(np.std(y)),xlabel="Time (second)",ylabel="Amplitude (pixels)")
    
    ax1.set_ylim(np.asarray(ax.get_ylim())*0.004481907894736842)
    ax1.set_ylabel("Amplitude (°)")
    ax.legend()
    plt.tight_layout()
    fig.savefig(join(P,"guided.png"))
    
    #print stats
    ls = natsorted([join(P,f) for f in listdir(P) if all(['guided' not in f,'.csv' in f])])
    for h in ls:
        S = tostring(h)
        _,_,y = get_data(h)
        std = np.std(y)
        print("%s ~~> %.1f pixels"%(S,std))
    
    P = [float(h.split('/')[-1].replace(".csv","").split("-")[0]) for h in ls]
    P = set(P)
    for p in P:
        fig,ax = plt.subplots()
        ax.plot(t_ug,y_ug,alpha=0.5,label="unguided")
        ax.plot(t_ug,y_ug,'o',alpha=0.5,label="unguided")
        for f in ls:
            if "/%.2f-"%p in f:
                t,x,y = get_data(f)
                ax.plot(t,y-np.median(y),alpha=0.5,label=tostring(f))
        ax.set(title="Performance PID",xlabel="Time (second)",ylabel="Amplitude (pixels)")
        ax1 = ax.twinx()
        ax1.set_ylim(np.asarray(ax.get_ylim())*0.004481907894736842)
        ax.legend()
        plt.tight_layout()
    
    
    
    """
    plt.show()
    
    
    
