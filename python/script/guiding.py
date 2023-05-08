#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Feb 13 22:21:19 2023

@author: espressjo
"""

from moteurs2 import moteur 
from guideCam2 import guideCam 
import numpy as np
from time import time,sleep
from PID import PID
from code_labjack import Labjack_lim
from sys import stderr

class guiding:
    def __init__(self):
        self.a=1
        self.inst = (548,990)
        self.fov = (96,100)#width, height width must be multiple of 8, height a multiple of 2
        self.plate_scale = 16.1/3600.0 #degree per pixel
        self.tick = time()
        self.pid_x = PID(kp=0.15,kd=0,ki=0.01) # PID rm-8
        self.pid_y = PID(kp=0.2,kd=0,ki=0.05) # PID tmax-6
        self.frequency = 10 #Hz slow down the guiding loop, it's useless to re-analyse the CM if we don't have a new images
        self._ite_time = 1.0/self.frequency
    
    def wait(self,already_elapsed_time:float):
        """
        This wait function will stall the guiding loop such that the minimum
        loop iteration frequency is at most self.frequency Hz.

        Parameters
        ----------
        already_elapsed_time : float
            Time already ellapsed for the previous loop.

        Returns
        -------
        New actual_spf.

        """
        t1 = time()
        cumul=0
        while((already_elapsed_time+cumul)<self._ite_time):
            sleep(0.01)
            cumul+=time()-t1
        return already_elapsed_time+cumul
            
    def guide(self):
        """
        Main algorythm for the coarse guiding. 

        Returns
        -------
        None.

        """
        with moteur() as motor:
            with guideCam() as gc:
                x,y = self.inst 
                width,height = self.fov
                gc.set_abs_expt(96)
                gc.set_AOI(x,y,width,height)
                c_map = self.create_coordinates_map()
                centerx,centery = self.inst
                while(1):
                    actual_spf = self.wait(time() - self.tick )# Monitors time for PID functions
                    self.tick = time()
                    cx,cy = gc.get_cm(c_map=c_map)
                    if np.isnan(cx) or np.isnan(cy):
                        continue 
                    y = (cy - centery)*self.plate_scale # delta y in degree
                    x = (cx- centerx)*self.plate_scale # delta x in degree
                    ox = self.pid_x.step(x, actual_spf)#Pass the error trough the PID
                    oy = self.pid_y.step(y, actual_spf)
                    motor.move_loop(oy,ox)                    
                                    
                    
    def create_coordinates_map(self):
        """
        Create a coordinate map.
        
        Parameters
        ----------
        width : INT
            DESCRIPTION.
        height : INT
            DESCRIPTION.

        Returns
        -------
        None.

        """
        lower_x = int(self.inst[0]-self.fov[0]/2.)
        lower_y = int(self.inst[1]-self.fov[1]/2.)
        c_map = np.zeros((2,self.fov[1],self.fov[0]))
        for x in range(lower_x,lower_x+self.fov[0]):
            for y in range(lower_y,lower_y+self.fov[1]):
                c_map[0,y,x]=x
                c_map[1,y,x]=y
        return c_map
    def set_fov(self,width:int,height:int):
        """
        Set the fov of the coarse guiding. 96X100 px is the recommended values.
        Only 1 star should be visible inside the FOV. A bigger FOV will affect
        the speed at which the centroid is computed.
        
        

        Parameters
        ----------
        width : int
            Must be a multiple of 8.
        height : int
            Must be a multiple of 2.

        Returns
        -------
        TYPE
            0-> success
            -1-> failed

        """
        if width%8!=0:
            return -1
        if height%2!=0:
            return -1
        self.fov = (width,height)
        return 0
    def set_inst_pos(self,x:int,y:int):
        """
        Set a position on the fine guiding cam FOV in the coarse guiding Cam FOV.
        i.e., When a star is position on the X,Y in the coarse guiding cam, you
        will be able to see it in the fine guiding cam.

        Parameters
        ----------
        x : INT
            DESCRIPTION.
        y : INT
            DESCRIPTION.

        Returns
        -------
        None.

        """
        self.inst = (x,y)
def guide():
    g = guiding()
    g.guide()

        
if '__main__' in __name__:
    with Labjack_lim(connection="Ethernet") as lim:
        h,zero = lim.anySwitchActivated()
        if h:
            print("Some lim. switch are activated",file=stderr)
            exit(0)
        lim_switch,return_code = lim.execute_with_limswitch(guide)
        if lim_switch!="":
            exit(0)
        else:
            print("%s lim. switch activated in the process",file=stderr)
            exit(1)
        