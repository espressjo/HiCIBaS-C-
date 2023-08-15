#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Aug  7 15:14:07 2023

@author: espressjo
"""
from nutec import nutec
from rm8 import rm8
from time import sleep
from sys import argv
from guideCam import coarseCam
from simple_pid import PID
from arguments import get_arg_int
from time import perf_counter as pc
import numpy as np
from shm_HiCIBaS import guiding

ALT = 35.25#steps/pixels
AZ = 51.24999999999999 #theoritical calculation
LIM_FREQ = 20 #maximum limit for the guiding loop
RM8_FREQ = 15
RM8_HSPEED = 4e4
RM8_LSPEED = 2e4
RM8_ACC = 2.6e4
NUTEC_SPEED_RPM = 0.3
NUTEC_ACC_RPS = 15
NUTEC_DEC_RPS = 100
NUTEC_FREQ = 10
NUTEC_PID = (0.3,0.01,0) #PID

class guiding_loop():
    def __init__(self):
        print("ok")
    def __exit__(self,a,b,c):
        try:
            G = guiding()
            G.guiding_nutec = False
            G.guiding_rm8 = False
        except:
            pass
    def __enter__(self):
        self.__init__()
        return self
    def set_rm8(self):
        """
        Set RM8 motor parameters
    
        Returns
        -------
        None.
    
        """
        print("setting RM8")
        with rm8("localhost",7565) as rm:
            sleep(0.4)
            rm.set_high_speed(int(RM8_HSPEED))
            sleep(0.4)
            rm.set_low_speed(int(RM8_LSPEED))
            sleep(0.4)
            rm.set_acceleration(int(RM8_ACC))
    
    def set_nutec(self):
        """
        Set Nutec motor parameters
    
        Returns
        -------
        None.
    
        """
        with nutec("localhost",7555) as n:
            print("setting Nutec")
            sleep(0.4)
            n.set_speed_rpm(NUTEC_SPEED_RPM)
            sleep(0.4)
            n.set_acc_rps(NUTEC_ACC_RPS)
            sleep(0.4)
            n.set_dec_rps(NUTEC_DEC_RPS)
          
    def guiding_loop(self,X,Y):
        # setting everything
        self.set_nutec()
        self.set_rm8()
        pid_nutec = PID(NUTEC_PID[0],NUTEC_PID[1],NUTEC_PID[2],setpoint=X)
        pid_nutec.sample_time = 1.0/NUTEC_FREQ
        #set PID options True/False (check hicibas-clone!!!!!)
        with coarseCam() as cam:
            """
            Set the FOV here!!!!
            """
            
            with nutec("localhost",7555) as nu:
                with rm8("localhost",7565) as rm:
                    t2 = 2
                    t1 = 0
                    while(1):
                        t1 = pc()
                        if (t2-t1)<(1.0/LIM_FREQ):
                            sleep(0.01)
                            t2 = pc()
                            continue                  
                        x,y = cam.get_moment()
                        to = t2-t1
                        if ( to > (1.0/NUTEC_FREQ) and not np.isnan(x)):
                            x_err = pid_nutec(x)
                            guiding.guiding_nutec = True
                            nu.move_no_return(int(x_err*ALT))
                        elif np.isnan(x):
                            guiding.guiding_nutec = False
                            
                        if (to > (1.0/RM8_FREQ) and not np.isnan(y)):
                            y_err = Y-y
                            guiding.guiding_rm8 = True
                            rm.move_no_return(int(y_err*AZ))
                        elif np.isnan(y):
                            guiding.guiding_nutec = False
                        t2 = pc()
    def guiding_loop_sub(self,X,Y,size):
        #X and Y are where we want our star
        #x_sub,y_sub,size is the subregion we want to consider
        # setting everything
     
        self.set_nutec()
        self.set_rm8()
        pid_nutec = PID(NUTEC_PID[0],NUTEC_PID[1],NUTEC_PID[2],setpoint=X)
        pid_nutec.sample_time = 1.0/NUTEC_FREQ
        #set PID options True/False (check hicibas-clone!!!!!)
        with coarseCam() as cam:
            """
            Set the FOV here!!!!
            """
            
            with nutec("localhost",7555) as nu:
                with rm8("localhost",7565) as rm:
                    t2 = 2
                    t1 = 0
                    while(1):
                        t1 = pc()
                        if (t2-t1)<(1.0/LIM_FREQ):
                            sleep(0.01)
                            t2 = pc()
                            continue                  
                        x,y = cam.get_moment_sub(X,Y, size)
                        to = t2-t1
                        if ( to > (1.0/NUTEC_FREQ) and not np.isnan(x)):
                            x_err = pid_nutec(x)
                            guiding.guiding_nutec = True
                            nu.move_no_return(int(x_err*ALT))
                        elif np.isnan(x):
                            guiding.guiding_nutec = False
                            
                        if (to > (1.0/RM8_FREQ) and not np.isnan(y)):
                            y_err = Y-y
                            guiding.guiding_rm8 = True
                            rm.move_no_return(int(y_err*AZ))
                        elif np.isnan(y):
                            guiding.guiding_nutec = False
                        t2 = pc()
    
if "__main__" in __name__:
    X = get_arg_int("--x=")
    Y = get_arg_int("--y=")
    #if you want the whole image (will probably not work)
    #with guiding_loop() as Guiding:
    #    Guiding.guiding_loop(X,Y)
    #sub region of 200px square
    with guiding_loop() as Guiding:
        Guiding.guiding_loop_sub(X,Y,200)
