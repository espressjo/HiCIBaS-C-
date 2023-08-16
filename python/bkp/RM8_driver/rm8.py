#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Dec 23 15:41:08 2022

@author: espressjo
"""

from ctypes import CDLL,c_int,create_string_buffer,byref
from os.path import isfile
from sys import stderr

class rm8():
    """
    Description
    -----------
    Python wrapper for the librm8 C library. 
    
    NOTES
    -----
        Make sure the library is compiled and installed properly
        before importing thing module.
    
    """
    def __init__(self):
        self.lfile = "/opt/HiCIBaS/log/.rm8.last.position"
        self.valid_position = -1
        #load the dynamic library
        self.rm8 = CDLL("/opt/HiCIBaS/lib/librm8.so")
    def _is_float(self,s:str):
        s = s.strip()
        for c in s:
            if not c.isnumeric() and c!='-' and c!='.':
                return False
        return True
                
    def upload_last_position(self):
        if isfile(self.lfile):
            with open(self.lfile,"r") as f:
                pos = f.read()
                if self._is_float(pos):
                    return self.setPosition(int(float(pos)))
        return -1
    def write_lst_pos(self):
        p = self.read_position()
        if self.valid_position!=0:
            print("Unable to write last position",file=stderr)
            open(self.lfile,"w").close()#erase the last number
            return -1
        with open(self.lfile,"w") as f:
            f.write(str(p))
        return 0
    def sndcmd(self,cmd:str):
        """
        Send and read back a command to the RM8 motor

        Parameters
        ----------
        cmd : str
            Command name. Refer to section 10. of the NSC-A1 manual.

        Returns
        -------
        STR
            Return of the command.

        """
        w = create_string_buffer(64)
        w.value = cmd.encode()
        r = create_string_buffer(64)
        self.rm8.sndCmd(w,r)
        return r.value.decode()
    def old_move(self,x):
        #deprecated
        return self.rm8.old_move(x)
    def old_loop(self,x):
        #deprecated
        return self.rm8.old_loop(x)
    def setPosition(self,n_position):
        """
        Set a new position as the absolute position.
        
        Return
        ------
            0->Success
            1->comm. problem
            2-> Cmd. failed
        """
        return self.rm8.setPosition(n_position)
    def move(self,x):
        """
        Make a move with the motor with the following configuration
        LSPD=5000, HSPD=10000 and ACC=300. This function return 
        immediatly.

        Parameters
        ----------
        x : INT
            Relative position in pulse.

        Returns
        -------
        0 -> successfull
        -1 -> failed to move the motor, the motor is probably already in motion

        """
        ret = self.rm8.move(x)
        return ret
    def move_wait(self,x):
        """
        Make a move with the motor with the following configuration
        LSPD=5000, HSPD=10000 and ACC=300. This function will perform
        the movement then return the new positon

        Parameters
        ----------
        x : INT
            Relative position in pulse.

        Returns
        -------
        n_pos: INT
            new actual position
        valid: BOOL
            If the position is valid, return True

        """
        n_pos = c_int()
        ret = self.rm8.move_wait(x,byref(n_pos))
        if ret!=0:
            return 0,False
        return n_pos,True
    def connect(self):
        """
        Establish a connection with the NSC-A1 controller.

        Returns
        -------
         *	0 connection established successfully
         * 	1 COMM error
         * 	2 No motor found
         * 	3 Generic Error, check stderr
         *  4 CMD failed

        """
        ret = self.rm8.connect() 
        self.upload_last_position()
        return ret 
    def status(self):
        """
        Read the motor status;
        
        0   motor is idle and all input switches are off
        1   Motor running at constant speed
        2   motor is in acceleration.
        4   Motor in deceleration
        8   Home input switch status
        16  Minus limit input switch status
        32  Plus limit input switch status
        64  Minus limit error. This bit is latched when minus
            limit is hit during motion. This error must be cleared
            using the CLR command before issuing any
            subsequent move commands.
        128 Plus limit error. This bit is latched when plus limit is
            hit during motion. This error must be cleared using
            the CLR command before issuing any subsequent
            move commands.
        256 Latch input status
        512 Z-index status
        1024 TOC time-out status

        Returns
        -------
        None.

        """
        mst = c_int()
        self.rm8.status(byref(mst))
        return mst.value
    def disable(self):
        """
        Disable the motor

        Returns
        -------
        INT
            < 0 fail
            0 -> success

        """
        return self.rm8.disable_drive()
    def abort(self):
        """
           Description
         * -----------
         * 	When motor is moving, jogging, or homing, 
         *  using the ABORT command will immediately stop 
         *  the motor.
         *  
         * 
         * Return
         * ------
         * 	0 if successfull
         * 	1 if we have a communication error
         * 	2 motor is probably not moving

        """
        return self.rm8.stopall()
    def enable(self):
        """
        Enable the motor

        Returns
        -------
        INT
            < 0 fail
            0 -> success

        """
        return self.rm8.enable_drive()
    def readInput(self):
        """
        Read the analogue 1 and analogue 2 input in mV

        Returns
        -------
        input 1 : FLOAT
            Input 1 in millivolts (-1 if unsuccessfull).
        input 2 : FLOAT
            Input 2 in millivolts (-1 if unsuccessfull).
            
        """
        I1 = create_string_buffer(64)
        I2 = create_string_buffer(64)
        self.rm8.getAnalogueInput(I1,I2)
        try:
            i1 = float(I1.value)
        except:
            i1=-1.0 
        try:
            i2 = float(I2.value)
        except:
            i2=-1.0
        return i1,i2
    def read_position(self):
        """
        Return the motor encoder value

        Returns
        -------
        FLOAT
            Encoder value.

        """
        position = create_string_buffer(64)
        self.valid_position = self.rm8.read_position(position)
        return float(position.value)
    def get_info(self):
        """
        Will return information on the RM8 motor

        Returns
        -------
        STR
            Device serial number
        STR
            device identification number.
        STR
            device name.

        """
        device_serial = create_string_buffer(256)
        device_id = create_string_buffer(64)
        device_name = create_string_buffer(64)
        self.rm8.get_info(device_serial,device_id,device_name)
        return device_serial.value.decode(),device_id.value.decode(),device_name.value.decode()
    def setParam(self,lspd,hspd,acc):
        """
         * Set the motor most common motor parameters
         *  HSPD -> high speed
         *  LSPD -> Low speed
         *  ACC  -> acceleration
         *
         * Return 
         * ------
         * 	0->success 
         *	1 - > motor is probably already in movement
         *  2 -> command failed.
        """
        return self.rm8.setMParameters(lspd,hspd,acc)
    def closeConnection(self):
        """
        * Description
        * -----------
        * Close the communication with the NSC-A1. 
        * 
        * NOTE
        * ----
        * we make sure the motor is not moving by sending
        * the ABORT command.
        * 
        * return 
        * ------
        * 0 -> successfull
        * 1 -> Communication problem
        * 2 -> Failed to send the command
        * 3 -> failed to disconnect
        """
        self.write_lst_pos()
        ret = self.rm8.closeConnection()
        return ret 
    def loop(self,x):
        """
         * Description
         * -----------
         * 	Try to perform an on-the-fly target position change, if 
         *  not successfull, we try to perform a simple position change.
         *  
         *  This function is meant to be used inside the guiding loop.
         *  It is not clear if we should use absolute movement in 
         *  combination with the get_position function for more accurate 
         *  results.



        Parameters
        ----------
        x : INT
            Relative movement in pulse.

        Returns
        -------
        0 if successfull
        1 if we have a communication error
        2 command failed.

        """
        return self.rm8.loop(x)
    def closem(self):
        self.rm8.closem()
if '__main__' in __name__:
    print("ok")       
