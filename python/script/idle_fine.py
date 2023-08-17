#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Aug 17 15:28:55 2023

@author: espressjo
"""

from guideCam import fineCam as C 
from time import sleep 



with C() as cam:
    while(1):
        cam.get_data()
        sleep(0.1)