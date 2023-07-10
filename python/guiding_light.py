#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Jul 10 12:08:00 2023

@author: espressjo
"""
import threading
from rm8 import rm8
from time import sleep
   

with rm8("localhost",7565) as m:
    m.enable_drive()
    m.set_low_speed(35*10000)
    m.set_high_speed(90*10000)
    m.set_acceleration(30*10000)
    i=0
    while(1):
        while(m.isMoving()):
            sleep(0.1)
        i+=1
        print("Moving ...")
        if i%2==0:
            m.move(25*10000)
        else:
            m.move(-25*10000)

            


    
