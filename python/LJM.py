#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Jun 16 06:36:47 2023

@author: espressjo
"""

from labjack import ljm
import struct,socket
from sys import stderr,exit 

def eprint(error:str):
    print(error,file=stderr)
    
class LJM:
    def __init__(self,model="ANY",mode="ANY",serial="ANY"):
        self.model = model
        self.serial = serial
        self.mode = mode
        self.handle = None
        self.info = None
    def isInitialized(f):
        def func(*args, **kwargs):
            if args[0].handle==None:
                eprint("lab jack not initialized.")
                exit(0)
            return f(*args, **kwargs)
        return func
    def __enter__(self):
        self.connect()
        return self
    def __exit__(self,a,b,c):
        self.disconnect()
        return
    def connect(self):
        self.handle = ljm.openS(self.model, self.mode, self.serial)
        self.info = ljm.getHandleInfo(self.handle)
    def disconnect(self):
        ljm.close(self.handle)
    def write(self,outputs,values):
        """
        Write an input or several inputs. If the input is an analog input e.g., AIN0, 
        The value will be between 0.0 - 5.0 V. Otherwise, the value will be 0.0 or 1.0.
        
        Parameters
        ----------
        outputs : str or list
            Name of the outputs.
        values : str or list
            values to be set..

        Returns
        -------
        INT
            0 -> successfull
            -1 -> failed.

        """
        if isinstance(outputs,list):
            assert len(outputs) == len(values)
            ljm.eWriteNames(self.handle, len(outputs), outputs, values)
            return 0
        elif isinstance(outputs,str):
            ljm.eWriteNames(self.handle, outputs, values)
            return 0
        else:
            return -1
    @isInitialized
    def read(self,inputs):
        """
        Read an input. If the input is an analog input e.g., AIN0, 
        The value returned will be between 0.0 - 5.0 V. Otherwise,
        the value will be 0.0 or 1.0.

        Parameters
        ----------
        inputs : str or list
            Name of the input.

        Returns
        -------
        FLOAT
            Value read from input or list of value read from inputs.

        """
        if isinstance(inputs, str):
            return ljm.eReadName(self.handle, inputs)
        elif isinstance(inputs, list):
            return ljm.eReadNames(self.handle, len(inputs), inputs)
        else:
            return None
    @isInitialized   
    def write_dac(self, DACNAME, tension:float):
        """
        Set a voltage on the given DAC. If using a LJTick-DAC module, 
        the nomenclature is as follow; Even TDAC# = DACA, Odd TDAC# = DACB. 
        For instance, if LJTick-DAC accessory is connected to FIO2/FIO3 
        block on main device, TDAC2 corresponds with DACA, and TDAC3 
        corresponds with DACB. 

        Parameters
        ----------
        DACNAME : STR
            Name of the DAC e.g., DAC1 or TDAC2.
        tension : FLOAT
            Tension in volts

        Returns
        -------
            -1 -> Error
            0 -> success

        """
        if 'DAC' not in DACNAME:
            return -1
        ljm.eWriteName(self.handle, DACNAME, tension)
        return 0
    @isInitialized
    def read_ip_address(self):
        """
        Return the Labjack IP address

        Returns
        -------
        ip : STR
            Labjack Ethernet connection IP address.

        """
        results = ljm.eReadAddress(self.handle, 49100, ljm.constants.UINT32)
        ip = socket.inet_ntoa(struct.pack('>L',int(results)))
        return ip
    @isInitialized
    def write_ip_address(self,ip):
        """
        Set a new Labjack IP address. You may need to power cycle the labjack.
        This operation will take a few second to take effect.

        Returns
        -------
        ip : STR
            Labjack Ethernet connection IP address.

        """
        ip_uint32 = socket.inet_aton(ip)
        ip_uint32 = struct.unpack("!L", ip_uint32)[0]
        ljm.eWriteAddress(self.handle, 49150, ljm.constants.UINT32,int(ip_uint32))
        ljm.eWriteAddress(self.handle, 49160, ljm.constants.UINT16,0)
        ljm.eWriteAddress(self.handle, 49190, ljm.constants.UINT32,1)
        return 0
    
if '__main__' in __name__:
    with LJM(mode="Ethernet") as ljm:
        print(f"Input AIN1: {ljm.read('AIN1')}")