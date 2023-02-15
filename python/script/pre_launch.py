#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Dec 22 10:47:03 2022

Description
============

    This script will move the telescope rigth by 2 degree,  up by 2 degree
    then move left and finally down by 2 degree. This script is mainly 
    used to check if everything is working fine regarding the telescope
    mount. It will print the absolute displacement in Azimuth and altitude
    in degree for diagnostic purpose.
    
NOTE
----
    This script uses the motors (RM8 and Nutec) as well as the labjack.

@author: espressjo
"""

from code_labjack import Labjack_lim
from moteurs2 import moteurs
from sys import exit,stderr
import logging
from time import sleep

def eprint(error:str):
    #print an error
    print(error,file=stderr)


def pre_launch():
    """
    We will move the az. and alt. by 2 degree and back to the original
    position

    Returns
    -------
    None.

    """
    with moteurs() as mot:
        az_1,alt_1 = mot.get_position()
        mot.move(-2, -2, wait=True)
        az_2,alt_2 = mot.get_position()
        mot.move(2, 2, wait=True)
        az_3,alt_3 = mot.get_position()
        az_a = abs(az_1-az_2)/10000.
        alt_a = abs(alt_1-alt_2)/10000.
        az_b = abs(az_2-az_3)/10000.
        alt_b = abs(alt_2-alt_3)/10000.
        print(f"(1) Altitude {alt_a} degree up, {alt_b} degree down")
        print(f"(2) Azimuth {az_a} degree left, {az_b} degree right")
        logging.warning('pre-launch sequence over') 
        exit(0)

if '__main__' in __name__:
    
    #:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    #:: 1st we want to see if any of the switch is activated  ::  
    #:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        
    with Labjack_lim(connection="Ethernet") as lbj:
        h_switch,zero_switch = lbj.anySwitchActivated()
        if h_switch:
            eprint("Cannot start the find_zero sequence. Hardware limit switch activated")
            exit(1)
    lim_switch,return_code = lbj.execute_with_limswitch(pre_launch)
    if lim_switch=='':
        exit(0)
    else:
        sleep(0.5)
        with moteurs() as mot:
            mot.move_from_limit(lim_switch)
        eprint("Failed, a limit switch has been activated")
        exit(1)
    


