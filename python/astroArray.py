#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Aug 13 11:12:17 2023

@author: espressjo
"""
import numpy as np
from cv2 import moments
from astropy.stats import sigma_clipped_stats as sc
from photutils.centroids import centroid_quadratic,centroid_2dg

class astroArrayClass(np.ndarray):
    def __init__(self):
        self._x = 0
        self._y = 0
        self.map = False
        
    def get(self,x,y):
        #return the original coordinate given the new ROI
        #coordinate
        return self._x+x,self._y+y
    def roi(self,x,y,size):
        size = int(size/2)
        y1 = y-size
        y2 = y+size
        x1 = x-size
        x2 = x+size
        if y1<0:
            y1=0
        if x1<0:
            x1=0
        if y2>=self.shape[0]:
            y2=self.shape[0]-1
        if x2>=self.shape[1]:
            x2=self.shape[1]-1
        A = np.asarray(self[y-size:y+size,x-size:x+size])
        A = A.view(astroArrayClass)
        A._x = x-size
        A._y = y-size
        A.map = True
        return A
    def moment(self,relatif=False):
        A = np.copy(self)
        mn,_,std = sc(A)
        A[A<(mn+4*std)]=0

        M = moments(A)
        if M["m00"]==0:
            return np.nan,np.nan
        cX = int(M["m10"] / M["m00"])
        cY = int(M["m01"] / M["m00"])
        if relatif:
            return cX,cY
        else:
            return self._x+cX,self._y+cY

    def gaus_2d(self,relatif=False):
        m = np.zeros(self.shape,dtype=bool)
        mn,_,std = sc(self)
        m[self<(mn+5*std)]=True
        x,y = centroid_2dg(self,mask=m)
        if relatif:
            return x,y
        else:
            return self._x+x,self._y+y
    def quad(self,relatif=False):

        x,y = centroid_quadratic(self,fit_boxsize=7)
        if relatif:
            return x,y
        else:
            return self._x+x,self._y+y
    
def astroArray(array,dtype=float):
    A = np.asarray(array,dtype=dtype)
    return A.view(astroArrayClass)