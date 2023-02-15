#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Dec 22 10:47:03 2022

Description
============

    This script will try to "home" the telescope. The home position
    is where the azimuth zero lim. switch is activated (0 degree) 
    and where the altitude lim. switch is activated (horizontal).
    You can have a look at the algorithm description in the 
    documentation for a better understanding and for troubleshooting.
Note
----
    This script may take up to 2-3 minutes to run to completion
    

@author: espressjo
"""

from code_labjack import Labjack_lim
from moteurs2 import moteurs
from sys import exit,stderr
from time import sleep

def eprint(error:str):
    #print an error
    print(error,file=stderr)
def move_alt_plus():#down
    """
    We will move the altitude. and alt. by 90 degree down. 
    """
    with moteurs() as mot:
        mot.move(0, 90,wait=True)
def move_alt_minus():#down
    """
    We will move the altitude. and alt. by 40 degree up.
    """
    with moteurs() as mot:
        mot.move(0, -40,wait=True)
    
def move_az_plus():
    """
    We will move the azimuth right by 35 degree. 
    """
    with moteurs() as mot:
        mot.move(35, 0,wait=True)
def move_az_minus():
    """
    We will move the azimuth left by 35 degree. 
    """
    with moteurs() as mot:
        mot.move(-35, 0,wait=True)

if '__main__' in __name__:
    #:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    #:: 1st we want to see if any of the switch is activated  ::  
    #:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        
    with Labjack_lim(connection="Ethernet") as lbj:
        h_switch,zero_switch = lbj.anySwitchActivated()
        if h_switch:
            eprint("Cannot start the find_zero sequence. Hardware limit switch activated")
            exit(0)
        if zero_switch:     
            active_switch = lbj.check_lim_switch()
            if 'az_zero' in active_switch and 'alt_zero' in active_switch:
                print("Telescope already homed")
                exit(0)
    #::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    #:: move the azimuth motor to the right, we don't want to skip the az zero lim. switch ::
    #::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        
    with Labjack_lim(connection="Ethernet") as lbj:
        lim_switch,return_code = lbj.execute_with_limswitch(move_az_plus,skip_az_zero=False)
        if 'right' in lim_switch:
            #we want to move left by 2 degree, then move left to find the zero
            with moteurs() as mot:
                print(lim_switch)
                mot.move_from_limit(lim_switch)
            sleep(0.3)
            lim_switch,return_code = lbj.execute_with_limswitch(move_az_minus,skip_az_zero=False)
            if "az_zero" not in lim_switch:
                eprint("Unexpected lim. switch activated or Az. zero lim. switch not found")
                exit(0)
            else:
                print("Azimuth zero lim. switch found successfully")
        elif 'az_zero' in lim_switch:
            print("Azimuth zero lim. switch found successfully")
        else:
            eprint("Unexpected lim. switch activated.[Az routine]")
            exit(1)
    
    #:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    #:: move the Altitude motor lower, we don't want to skip the alt zero lim. switch ::
    #:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        
    with Labjack_lim(connection="Ethernet") as lbj:
        lim_switch,return_code = lbj.execute_with_limswitch(move_alt_plus,skip_alt_zero=False)  
        if 'lower' in lim_switch:
            #we want to move up by 2 degree, then move up to find the zero switch
            with moteurs() as mot:
                print(lim_switch)
                mot.move_from_limit(lim_switch)
            sleep(0.3)
            lim_switch,return_code = lbj.execute_with_limswitch(move_alt_minus,skip_alt_zero=False) 
            if "alt_zero" not in lim_switch:
                eprint("Unexpected lim. switch activated or Alt. zero lim. switch not found")
                exit(0)
            else:
                print("Altitude zero lim. switch found successfully")
                exit(0)
        elif 'alt_zero' in lim_switch:
            print("Altitude zero lim. switch found successfully")
            exit(0)
        else:
            eprint("Unexpected lim. switch activated. [alt routine]")
            exit(1)



