#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Jun 14 10:30:00 2023

@author: espressjo
"""

from telnetlib import Telnet
from time import sleep
import traceback

class myTelnet:
    def __init__(self,host,port):
        self.port = port 
        self.host = host
        self.connection_timeout =5# 5 seconds
        self.cmd_delay = 0.05 #sleep before each write call. 
        
    def connect(self):
        '''
        Initiate a connection with HxRG-SERVER

        Returns
        -------
        None.

        '''
        self.tn = Telnet(timeout=self.connection_timeout)
        self.tn.open(self.host,self.port)
        if 'ECHO' not in self.read():
            print("impossible to connect to the server. Cheack your setup.")
            exit(0)
    def _2bytes(self,string):
        '''
        Description
            We must encode every cmd before sending them with telnet
        '''
        return string.encode()
    def _read(self,bytes_msg):
        '''
        Description
            we must decode the output of telnet (from bytes 2 utf-8)
        '''
        return bytes_msg.decode('utf-8').strip()
    def read(self):
        sleep(self.cmd_delay)
        output = self.tn.read_until(b'\n')
        return self._read(output)
    def readtxt(self):
        r = self.read()
        txt = ''
        while(1):
            if not r:
                return txt.replace('OK','') 
            txt+=r+"\n"
            r = self.read()
    def write(self,msg):
        '''
        Description:
            writes a msg to telnet
        '''
        
        self.tn.write(self._2bytes(msg))
        return 0
    def close(self):
        self.tn.close()
    def __enter__(self):
        '''
        Since connection to hardware is made, its better to use with statement i.e., with beckoff("192.168.62.150") as beck: ...

        Returns
        -------
        SELF
        '''
        self.connect()
        return self

    def __exit__(self, exc_type, exc_value, tb):
        
        self.tn.close()
        if exc_type is not None:
            traceback.print_exception(exc_type, exc_value, tb)
            # return False # uncomment to pass exception through

        return True           
    def write_get_int(self,cmd:str,default=-9999):
        """
        This function perform a write and read and return a int

        Parameters
        ----------
        cmd : str
            Command to be sent.
        default : INT, optional
            Default value if received NOK. The default is -9999.

        Returns
        -------
        INT
            Value returned.

        """
        self.write(cmd)
        ret = self.read()
        if "NOK" in ret:
            return default
        else:
            try:
                return int(ret.replace("OK ",""))
            except:
                return default
    def write_get_float(self,cmd:str,default=-999.9):
        """
        This function perform a write and read and return a float

        Parameters
        ----------
        cmd : str
            Command to be sent.
        default : float, optional
            Default value if received NOK. The default is -999.9.

        Returns
        -------
        float
            Value returned.

        """
        self.write(cmd)
        ret = self.read()
        if "NOK" in ret:
            return default
        else:
            try:
                return float(ret.replace("OK ",""))
            except:
                return default
    def write_get_str(self,cmd:str,default="NULL"):
        """
        This function perform a write and read and return a string

        Parameters
        ----------
        cmd : str
            Command to be sent.
        default : STR, optional
            Default value if received NOK. The default is "NULL".

        Returns
        -------
        STR
            Value returned.

        """
        self.write(cmd)
        ret = self.read()
        if "NOK" in ret:
            return default
        else:
            try:
                return str(ret.replace("OK ",""))
            except:
                return default    
         