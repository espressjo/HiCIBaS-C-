#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Jul 12 09:04:22 2023

@author: espressjo
"""

from rm8 import rm8

RM8START = 0
RM3START = 0

with rm8("localhost",7565) as m:
    p = m.get_pos()
    diff = RM8START-p
    m.set_low_speed(5000)
    m.set_high_speed(10000)
    m.set_acceleration(300)
    m.move(diff)

with rm8("10.9.32.206",7565) as rm3:
    p = rm3.get_pos()
    diff = RM3START-p
    rm3.move(diff)