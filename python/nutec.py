#!/usr/bin/python3

"""
File: code_nutec.py
Author: Copley controls
Modified by: Philippe Truchon
Date: 09/08/22  
Desc: This file contains the methods to communicate with the nutec controller   
state: WORK IN PROGRESS
"""

# use Python Version 3.8

# The following script is an example of how to send ASCII commands
# as well as serial binary commands to the drive.

import serial
from sys import exit
from HiCIBaS_CONF import LOGPATH,NUTEC_SERIAL_PORT,NUTEC_BAUDRATE
from os.path import join
from Hlog import LHiCIBaS 

log = LHiCIBaS(__file__)
class nutec:
    """
    Description
    -----------
        class to control the nutec motor
    
    Methods
    -------
        *enable_drive -> Enable the motor drive\n
        *disable_drive -> Disable the motor drive\n
        *log_last_position -> Log the last position on disk\n
        *move_l -> Perform a slow move for long movement\n
        *move_resp -> Perform a move with adjustable speed\n
        *loop -> Move to target position then return current position\n
        *get_pos -> return current position\n
        *close -> Close the serial communication and make sure the motor stopped moving\n
    """
    def __init__(self):

        self.ser = serial.Serial(NUTEC_SERIAL_PORT, baudrate=NUTEC_BAUDRATE, timeout=0.02)
        log.info("Serial comm. established")
    def set(self):       


        global error_bit
        error_bit = 0

        self.ser.baudrate = 9600

        # Disable the drive
        self.disable_drive()  # Set Desired State = 0

        """#print('Current loop limits') # List of all parameters that could be changed to test the performance
                                        # For the moment they are all in get mode to be safe about not changing them accidentally.
                                        # Refer to the ascii programmer's guide in documentation_externe to know what these mean

        self.SendAsciiCmd('g r0x21')

        self.SendAsciiCmd('g r0x23')

        self.SendAsciiCmd('g r0x22')

        self.SendAsciiCmd('g r0xae')

        #print('current loop gains')

        self.SendAsciiCmd('g r0x00')

        self.SendAsciiCmd('g r0x01')

        #print('Velocity loop limits')

        self.SendAsciiCmd('g r0x3a')

        self.SendAsciiCmd('g r0x36')

        self.SendAsciiCmd('g r0x37')

        self.SendAsciiCmd('g r0xcf')

        #print('velocity loop gains')"""

        #print(self.SendAsciiCmd('g f0x27')) #new value 30000

        #self.SendAsciiCmd('g f0x28') #new value  900

        #new filter value 4

        """#print('position loop limits')

        self.SendAsciiCmd('g r0xcb')

        self.SendAsciiCmd('g r0xcc')

        self.SendAsciiCmd('g r0xcd')

        self.SendAsciiCmd('g r0xce')

        self.SendAsciiCmd('g r0xcf')

        self.SendAsciiCmd('g r0xc8')

        #print('position loop gains')

        self.SendAsciiCmd('g r0x30')

        self.SendAsciiCmd('g r0x33')

        self.SendAsciiCmd('g r0x34')

        self.SendAsciiCmd('g r0xe3')"""

        self.SendAsciiCmd('s r0xcb 200000') # Sets velocity counts/second

        # Clear latched encoder faults
        #self.SendAsciiCmd('enc clear')


        # Clear latched fault status register
        #write 1 to current each bit to clear any errors
        self.SendAsciiCmd('s r0xa4 0xffff')

        # Enable the drive by setting the Desired State to Software Position Programmed Mode
        self.enable_drive()

        # Set the Trajectory Configuration to Velocity Mode
        #self.SendAsciiCmd('s r0xc8 2')

        # set Trajectory Configuration to relative move s-curve
        self.SendAsciiCmd('s r0xc8 257')

        # Send a serial binary command using a byte array.
        # Delete CVM Sequence 0 by sending 00 4f 02 14 00 03 00 00
        packet = bytearray([0x00, 0x4f, 0x02, 0x14, 0x00, 0x03, 0x00, 0x00])

        #print("Sending serial binary command to delete CVM Sequence 0")
        self.ser.write(packet)
        #print("Serial binary command sent")
        self.SerReadSerialBinaryResponse()
    def set_peak_current_limit(self,current:float):
        """
        Set the peak Current limits. Units: 0.01A

        Parameters
        ----------
        current : float
            Units: 0.01A.

        Returns
        -------
        ret : TYPE
            return serial responce.

        """
        ret = self.SendAsciiCmd("s r0x21 %.2f"%float)
        return ret
    def set_continuous_current_limit(self,current:float):
        """
        Set the continuous Current limits. Units: 0.01A

        Parameters
        ----------
        current : float
            Units: 0.01A.

        Returns
        -------
        ret : TYPE
            return serial responce.

        """
        ret = self.SendAsciiCmd("s r0x22 %.2f"%float)
        return ret
    def log_last_position(self):
        """
        Log on file the last position of the encoder

        Returns
        -------
        None.

        """
        last_position = self.SendAsciiCmd('g r0x32')
        with open(join(LOGPATH,'share.txt'), 'w') as f:
            f.write(last_position)    
            log.info(f"Last position nutec: {last_position}")
        
    def move_l(self,z):
        """
        Description
        -----------
            Perform a slow movent to position z. This
            function should be used for long movement.

        Parameters
        ----------
        z : TYPE
            Target position.

        Returns
        -------
        STR
            ok -> successful, 
            e <error#> -> failed

        """
        self.SendAsciiCmd('s r0xcb 50000')  #Slower speed for long moves    
        self.SendAsciiCmd(f's r0xca {z}') #Set wanted position
        return self.SendAsciiCmd('t 1') #Do the move
    def isMoving(self):
        """
        Check if the motor is moving. 

        Returns
        -------
        INT
            -1 -> problem with communication
            0 -> motor is NOT moving
            1 -> motor is moving

        """
        out = self.SendAsciiCmd('g r0xc9')
        
        print(out)
        if "v" not in out:
            log.critical("Nutec Comm. Err.")
            return -1
        out = out.strip('\r').replace('v ','')
        try:
            status = int(out)
        except:
            log.critical("Nutec Comm. Err.")
            return -1
        if status & 32768 ==32768:
            return 1 
        else:
            return 0
    def move_resp(self,z, speed = 200000):
        """
        Description
        -----------
            Perform a movent to position z. The speed can 
            be adjusted with <speed>

        Parameters
        ----------
        z : TYPE
            Target position.
        speed : INT
            Speed of the movement
        Returns
        -------
        STR
            ok -> successful, 
            e <error#> -> failed

        """       
        self.SendAsciiCmd(f's r0xcb {speed}')  #Input the speed for response test  
        self.SendAsciiCmd(f's r0xca {z}') #Set wanted position
        return self.SendAsciiCmd('t 1') #Do the move

    def loop(self, y):
        """
        Description
        -----------
            Perform a movent to position y. This function 
            should be used to "track" a star

        Parameters
        ----------
        y : TYPE
            Target position.

        Returns
        -------
        STR
            ok -> successful, 
            e <error#> -> failed
            

        """
        self.SendAsciiCmd(f's r0xca {y}') #Set wanted position
        self.SendAsciiCmd('t 1') #Do the move
        return self.SendAsciiCmd('g r0x0c') 

    def get_pos(self):
        """
        Description
        -----------
            return the current position of the encoder

        Returns
        -------
        position: INT
            position in steps.
        valid: BOOL
            True valid position, False failed to read position.

        """
        pos = self.SendAsciiCmd('g r0x32')
        if 'v' in pos:
            try: 
                p = int(pos.strip('\r').replace("v ",""))
                return p,True
            except:
                pass
        return 0,False  
    def is_in_fault(self):
        """
        Check if there is a fault with the controller

        Returns
        -------
        TYPE
            True/False

        """
        out = self.SendAsciiCmd('g r0xa0')
        if "v" not in out:
            log.critical("Nutec Comm. Err.")
            return -1
        out = out.strip('\r').replace('v ','')
        try:
            status = int(out)
        except:
            log.critical("Nutec Comm. Err.")
            return -1
        if status & 4194304 ==4194304:
            return True 
        else:
            return False
    def get_status_register(self):
        """
        Description
        -----------
            return the status register. See documentation for bit
        assignment

        Returns
        -------
        encoded value: INT
            

        """
        ecode = self.SendAsciiCmd('g r0xa0')
        return ecode
    def get_fault_error_code(self):
        """
        Description
        -----------
            return the fault error code

        Returns
        -------
        encoded value: INT

        """
        ecode = self.SendAsciiCmd('g r0xa4')
        return ecode

    def __del__(self):
        """
        If anything happen when for some reason we don't catch 
        the error and do stuff with the motor. Let's make sure
        we never leave the motor moving if it's the case and
        lets always make sure to close the serial communication

        Returns
        -------
        None.

        """
        try:
            self.SendAsciiCmd('t 0')#Stop the move
        except Exception:
            pass
        try:
            self.ser.close()
        except Exception:
            pass
    def enable_drive(self):
        """
        The controller will enable the drive.

        Returns
        -------
        None.

        """
        self.SendAsciiCmd('s r0x24 21')
    def disable_drive(self):
        """
        The controller will disable the drive in which case the 
        telescope will be free to move.

        Returns
        -------
        None.

        """
        self.SendAsciiCmd('s r0x24 0') #Disable the drive
    def stop(self):

        self.SendAsciiCmd('t 0')#Stop the move
    def close(self):
        """
        Close the connection with the motor controller. 
        The state of the drive (enabled or disabled) will
        not be affected by this call. If you want to disable
        the drive, explicitely call disable_drive() or 
        enable_drive()
        
        Returns
        -------
        None.

        """
        #Make sure the motor is stopped
        
        self.SendAsciiCmd('t 0')#Stop the move
        self.ser.close()
        log.info("Serial comm. closed with NUTEC")
       
    # Send an ASCII command and return the drive's response
    def SendAsciiCmd(self, cmd:str):
        """
        Send a command and read the response from the controller.

        Parameters
        ----------
        cmd : STR
            DESCRIPTION.

        Returns
        -------
        STR
            Response from the controller. Empty string might indicate 
            a problem with the serial communication.

        """
        #check if the cmd ends with return carrige
        if cmd[-1]!='\r':
            cmd = cmd + '\r'
        self.ser.write(cmd.encode())
        #print("ASCII Cmd Sent: " + cmd)
        return self.SerReadASCIIResponse()


    # Collects all ASCII responses from drive
    def SerReadASCIIResponse(self):
        """
        Uppon a successful command we should receive; ok.
        Empty string might suggest something is wrong with
        the communication.

        Returns
        -------
        ret : TYPE
            DESCRIPTION.

        """
        ret = ''
        while (True):
            ch = self.ser.read(1)
            if (len(ch) < 1 or ch == '\r'):
                #print("ASCII Response: " + ret)
                return ret
            ret += str(ch, 'utf-8')


    # Collects all serial binary response data
    def SerReadSerialBinaryResponse(self):
        ret = []
        while (True):
            ch = self.ser.read(1)
            if (len(ch) < 1 or ch == '\r'):
                #print("Serial Binary Response: " + (" ".join(ret)))
                return (ret)
            ch = ch.hex()
            ret.append(ch)
    def read_register(self,reg,RAM=True):
        if RAM:
            return self.SendAsciiCmd("g r%s"%reg)
        else:
            return self.SendAsciiCmd("g f%s"%reg)
    def write_register(self,reg,value,RAM=True):
        if RAM:
            return self.SendAsciiCmd(f"s r{reg} {value}")
        else:
            return self.SendAsciiCmd(f"s f{reg} {value}")
    def position_time(self,steps=200,timeout=60):
        """
        Used to plot a position vs time graph.

        Parameters
        ----------
        steps : INT, optional
            number of steps to perform for the study. The default is 200.

        Returns
        -------
        dt : TYPE
            DESCRIPTION.
        p : TYPE
            DESCRIPTION.

        """
        from tqdm import tqdm
        from time import sleep
        n = nutec()
        start = n.get_pos()
        dt = [];
        p = [];
        from datetime import datetime
        p.append(start[0])
        dt.append(datetime.now())
        print("Starting position: ",n.get_pos())
        n.loop(steps)
        for i in tqdm(range(5*timeout)):
            sleep(0.2)
            pos = n.get_pos()
            p.append(pos[0])
            dt.append(datetime.now())
        import numpy as np
        dt = np.asarray(dt)
        from matplotlib import pyplot as plt 
        import seaborn as sns 
        sns.set_theme()
        fig,ax = plt.subplots()
        time = [(d-dt[0]).seconds for d in dt]
        end = p[-1]
        ax.plot(time,p,'o')
        ax.set(title="Start: %d, End: %d, Steps: %d"%(start[0],end,steps),xlabel="Time (seconds)",ylabel="Position (steps)")
        plt.tight_layout()
        plt.show()
        return dt,p,fig
if __name__ == "__main__":
    try:
        n = nutec()
        n.set()
    except KeyboardInterrupt:
        n.close()
        print('Interrupted')
        exit(0)