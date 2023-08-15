#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Aug 15 16:32:49 2023

@author: espressjo
"""

from guideCam import coarseCam

with coarseCam() as cam:
    info = cam.get_current_fov()
print(info)