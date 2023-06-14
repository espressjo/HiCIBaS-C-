#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Jun 14 10:29:31 2023

@author: espressjo
"""

from mytelnet import myTelnet

def vel_rpm(counts):
    return 0.1*counts*60/3.6e6
def vel_counts(rpm,unit=0.1):
    return int(rpm/60.0*3.6e6 /unit)
def acc_counts(rps,unit=10):
    return rps*3.6e6/unit
def acc_rps(count,unit=10):
    count*=unit
    return count/3.6e6
def jerk_counts(rps,unit=100):
    return rps*3.6e6/unit
def jerk_rps(count,unit=100):
    count*=unit
    return count/3.6e6



class nutec(myTelnet):
    def __init__(self,host,port):
       super().__init__(host, port)
    def isMoving(self):
        if "T" in self.write_get_str("isMoving"):
            return True
        else:
            return False
    def enable_drive(self):
        self.write("enable_drive")
        ret = self.read()
        if "NOK" in ret:
            return -1
        elif "OK" in ret:
            return 0
        else:
            return -1
    def stop(self):
        self.write("abort")
        ret = self.read()
        if "NOK" in ret:
            return -1
        elif "OK" in ret:
            return 0
        else:
            return -1
    def abort(self):
        self.write("abort")
        ret = self.read()
        if "NOK" in ret:
            return -1
        elif "OK" in ret:
            return 0
        else:
            return -1
    def disable_drive(self):
        self.write("disable_drive")
        ret = self.read()
        if "NOK" in ret:
            return -1
        elif "OK" in ret:
            return 0
        else:
            return -1
    def isEnabled(self):
        if "T" in self.write_get_str("isEnabled"):
            return True
        else:
            return False
    def move_abs(self,abs_pos:int):
        self.write(f"move_abs position {abs_pos}")
        ret = self.read()
        if "NOK" in ret:
            return -1
        elif "OK" in ret:
            return 0
        else:
            return -1
    def move(self,rel_pos:int):
        self.write(f"move position {rel_pos}")
        ret = self.read()
        if "NOK" in ret:
            return -1
        elif "OK" in ret:
            return 0
        else:
            return -1
    def get_pos(self,default=-99999999):
        return self.write_get_int("get_pos",default)
    def get_speed(self,default=-1):
        self.write_get_int("get_speed",default=default)
    def set_speed(self,speed:int):
        self.write(f"set_speed speed {speed}")
        ret = self.read()
        if "NOK" in ret:
            return -1
        elif "OK" in ret:
            return 0
        else:
            return -1
    def set_speed_rpm(self,speed_rpm:float):
        speed = vel_counts(speed_rpm)
        self.write(f"set_speed speed {speed}")
        ret = self.read()
        if "NOK" in ret:
            return -1
        elif "OK" in ret:
            return 0
        else:
            return -1
            
    
if '__main__' in __name__:
    with nutec("localhost",7555) as n:
        print(f"Position: {n.get_pos}")