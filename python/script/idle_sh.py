#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Aug 17 15:33:45 2023

@author: espressjo
"""
from time import sleep
from peak import lrio_peak 
with lrio_peak() as SH:#connect to hardware
    SH.start_acquisition
    while(1):
        SH.get_image()
        sleep(0.1)
    SH.stop_acquisition