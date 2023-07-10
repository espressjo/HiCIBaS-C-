#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jun 20 09:25:11 2023

@author: espressjo
"""

from mytelnet import myTelnet

class rm8(myTelnet):
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
    # def isEnabled(self):
    #     if "T" in self.write_get_str("isEnabled"):
    #         return True
    #     else:
    #         return False
    def move(self,rel_pos:int):
        self.write(f"move position {rel_pos}")
        ret = self.read()
        if "NOK" in ret:
            return -1
        elif "OK" in ret:
            return 0
        else:
            return -1
    def move_no_return(self,rel_pos:int):
        self.write(f"move position {rel_pos}")
        return 0
    def get_pos(self,default=-99999999):
        return self.write_get_int("get_pos",default)
    def get_low_speed(self,default=-1):
        return self.write_get_int("get_low_speed",default=default)
    def get_high_speed(self,default=-1):
        return self.write_get_int("get_high_speed",default=default)
    def set_low_speed(self,speed:int):
        self.write(f"set_low_speed speed {speed}")
        ret = self.read()
        if "NOK" in ret:
            return -1
        elif "OK" in ret:
            return 0
        else:
            return -1
    def set_high_speed(self,speed:int):
        self.write(f"set_high_speed speed {speed}")
        ret = self.read()
        if "NOK" in ret:
            return -1
        elif "OK" in ret:
            return 0
        else:
            return -1
    def set_acceleration(self,acc:int):
        self.write(f"set_acceleration acceleration {acc}")
        ret = self.read()
        if "NOK" in ret:
            return -1
        elif "OK" in ret:
            return 0
        else:
            return -1

            
    
if '__main__' in __name__:
    a=1
    
            
            
            
            
            
            
            
            