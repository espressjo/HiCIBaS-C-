#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Jun 19 23:45:26 2023

@author: espressjo
"""
from nutec import nutec
from rm8 import rm8
import threading

class moteurs2:
    def __init__(self):
        self.nutec = nutec("localhost",7555)
        self.rm8 = rm8("localhost",7565)
    def abort(self):
        self.rm8.abort()
        self.nutec.abort()
    def move(self,alt:int,az:int):
        self.nutec.move(alt)
        self.rm8.move(az)
    def move_t(self,alt:int,az:int):
        nutec_t = threading.Thread(target=self.nutec.move, args=(alt,))
        rm8_t = threading.Thread(target=self.rm8.move, args=(az,))
        nutec_t.start()
        rm8_t.start()
        nutec_t.join()
        rm8_t.join()
    def enable_drive(self):
        self.nutec.enable_drive()
        self.rm8.enable_drive()
    def disable_drive(self):
        self.nutec.disable_drive()
        self.rm8.disable_drive()    
    def get_position(self):
        alt = self.nutec.get_pos()
        az = self.rm8.get_pos()
        return alt,az        
    def connect(self):
        self.nutec.connect()
        self.rm8.connect()
    def __enter__(self):
        self.connect()
        return self 
    def __exit__(self,a,b,c):
        self.disconnect()
    def disconnect(self):
        self.nutec.disconnect()
        self.rm8.disconnect()
        
if '__main__' in __name__:
    with moteurs2 as M:
        alt,az = M.get_position()
        print(f"Alt: {alt}, Az: {az}")
        
        