#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Aug 18 17:28:12 2023

@author: espressjo
"""

from os.path import isfile,join 
from natsort import natsorted 
from os import listdir,popen 
from datetime import datetime 
fmt="%Y%m%d"
P = join("/opt/TRASH/data/",datetime.now().strftime(fmt))
#get last file name
ls = natsorted(listdir(P))

if len(ls)==0:
    print("No images")
f = join(P,ls[-1])

if isfile(f.replace(".fits",".fits.gz")):
    popen("rm -f %s"%("/var/tmp/trash.fits.gz")).read()
popen("cp %s /var/tmp/trash.fits").read()
popen("gzip /var/tmp/trash.fits").read()
print("/var/tmp/trash.fits.gz saved!")


