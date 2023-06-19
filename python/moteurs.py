#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Dec 21 15:52:06 2022


altitude: horizontal ~ 79387 (y-zero activated)
azimuth:  centered ~0 (x-zero activated)


@author: espressjo
"""
from nutec import nutec
from limswitch import Labjack_lim
from rm8 import rm8
import signal
from sys import stderr,exit,argv
from time import time,sleep
from PID import PID
from HiCIBaS_CONF import LOGPATH
import traceback
from os.path import join
from Hlog import LHiCIBaS
from shm_HiCIBaS import devices,telescope
from config_get import get_string

dev = devices()
tel = telescope()
log = LHiCIBaS(__file__)

def handler(signal_received, frame):
    """
    Whenever SIGTERM is emitted we raise a KeyboardInterupt
    to the current process.

    """
    raise KeyboardInterrupt()

class moteurs:
    """
    Description
    ===========
        Class to control both telescope mount motors.
    NOTE
    ----
        You can use the with moteurs as ... statement. Some 
        utility method are implemented to ensure __exit__ 
        is called.
    Methods
    -------
        * kill -> Close all the motors and exit\n
        * close -> Close all the motors.\n
        * connect -> Establish connection with both motors\n
        * get_position -> return position of both motors\n
        * enable -> Enable both motirs\n
        * disablee -> Disable both motors\n
        * move_az -> move azimuth in deg. \n
        * move_alt -> move altitude in deg.\n
        * isMoving -> True if motor is moving \n
        * move -> Move in alt. and az. in deg\n
    """
    def __init__(self):
        """
        Initialize both nutec and MR8 motors and make sure the labjack is
        connectected.

        Returns
        -------
        None.

        """
        #self.hicibas_shm = hicibas_shm()
        self.m_nutec = nutec()
        self.m_RM8 = rm8()
        
        #log.info('Motor info ((xerror, yerror), (tmax-6 current)) (units: error (arc-seconds), current(1=0.01A)): %s', ((0,0), 'v 0'))
        self.tick = time()
        self.pid_x = PID(kp=0.15,kd=0,ki=0.01) # PID rm-8
        self.pid_y = PID(kp=0.2,kd=0,ki=0.05) # PID tmax-6
        
        #init some stuff
        
    def connect(self):
        dev.rm8 = True
        if self.m_RM8.connect()!=0:
            print("Failed to start RM8",file=stderr)
            dev.rm8 = False
            exit(1)
        log.info('rm-8 ready')
        self.m_nutec.set()
        dev.nutec = True
        log.info('tmax-6 ready')
        
        #I really don't think we want to do that
        nu_pos,valid = self.m_nutec.get_pos()
        
        if not valid:
            print("unable to get nutec motor position",file=stderr)
            dev.nutec = False
            exit(1)
        tel.alt_encoder = int(nu_pos)
        self.landing_pos = str(nu_pos) # Stores the current position in a text file (the first one should be

        with open(join(LOGPATH,'landing_pos.txt'), 'a') as f: #Need to erase content before a launch (real starting position)
            f.write(self.landing_pos)
        sleep(2)
        log.info('moteurs instance created')
        return
    def __enter__(self):
        '''
        Since connection to hardware is made, its better to use with statement i.e., 
        with moteurs() as mot:
            mot.get_position()
            
        Returns
        -------
        SELF
        '''
        signal.signal(signal.SIGINT, self._handle_interrupt)
        signal.signal(signal.SIGTERM, self._handle_interrupt)
        self.connect()
        return self
    def _handle_interrupt(self,signal_received, frame):

        exit(1)

    def __exit__(self, exc_type, exc_value, tb): 
        alt,az = self.get_position()
        tel.az_encoder = az
        tel.alt_encoder = alt
        log.info("exit moteurs()")
        self.m_nutec.stop()
        self.m_RM8.abort()
        self.m_RM8.disable()
        sleep(0.1)
        self.m_nutec.log_last_position()
        self.m_nutec.close()
        self.m_RM8.closeConnection()
        tel.az = az/10000.0
        tel.alt = alt/10000.0
        dev.nutec = False
        dev.rm8 = False
        tel.nutec_moving = False
        tel.rm8_moving = False
        if exc_type is not None:
            traceback.print_exception(exc_type, exc_value, tb)

    def kill(self): 
        """
        Description
        -----------
            Emergency stop. This method will be the last thing the process
            will do since it call exit(1)

        Returns
        -------
        None.

        """
        
        sleep(0.1)
        alt,az = self.get_position()
        self.m_nutec.log_last_position()
        self.m_nutec.close()
        self.m_RM8.closeConnection()
        tel.az = az/10000.0
        tel.alt = alt/10000.0
        tel.az_encoder = az
        tel.alt_encoder = alt
        dev.nutec = False
        dev.rm8 = False
        #self.hicibas_shm.set_alt(alt/10000.0)
        #self.hicibas_shm.set_az(az/10000.0)
        #self.hicibas_shm.unset_alt_moving()
        #self.hicibas_shm.unset_az_moving()
        
        log.warning('Keyboard Interrupt processes have been closed')
        exit(1) #Le sys.exit(0) lève aussi une exception dans un script qui appelle la classe, par exemple launch.py

    def get_position(self):
        """
        Description
        ------------
            return the current encoder value for the RM8 and Nutec motors

        Returns
        -------
        int,int
            Azimuth encoder, ALtitude encoder

        """    
        n_pos,valid = self.m_nutec.get_pos()
        rm8_pos = self.m_RM8.read_position()
        #write stuff in shared memory
        tel.alt = n_pos/10000.0
        tel.alt_encoder = int(n_pos)
        tel.az = rm8_pos/10000.0
        tel.az_encoder = int(rm8_pos)
        return rm8_pos,n_pos
    def enable(self):
        """
        Enable both motors. 


        Returns
        -------
        None.

        """
        tel.nutec_enabled = True
        tel.rm8_enabled = True
        self.m_nutec.enable_drive()
        self.m_RM8.enable()
        return
    def disable(self):
        """
        Disable both motors. 
        
        Warnings
        --------
            The Nutec motor will be disangaged and the telescope will 
            be free to move.

        Returns
        -------
        None.

        """
        tel.nutec_enabled = False
        tel.rm8_enabled = False
        self.m_nutec.disable_drive()
        self.m_RM8.disable()
        return
    def close(self):
        """
        Description
        -----------
            Close the neccessary hardware (motors). Both motors 
            will stay enabled.

        """
        tel.rm8_moving = False
        tel.nutec_moving = False
        dev.nutec = False 
        dev.rm8 = False
        self.m_nutec.close()
        log.info('nutec movement stopped (motor enabled)')
        self.m_RM8.closeConnection()
        log.info('rm-8 closed')

    def move_from_limit(self,active_lim):
        if isinstance(active_lim, str):
            res = [active_lim]
        else:
            res = active_lim
        
        if 'left' in res:
            tel.rm8_moving = True
            self.move_az(2.2)
            while(any(self.isMoving())):
                sleep(0.2)
            tel.rm8_moving = False
        if 'right' in res:
            tel.rm8_moving = True
            self.move_az(-2.2)
            while(any(self.isMoving())):
                sleep(0.2)
            tel.rm8_moving = False
        if 'upper' in res:
            tel.nutec_moving = True
            self.move_alt(2)
            while(any(self.isMoving())):
                sleep(0.2)
            tel.nutec_moving = False
        if 'lower' in res:
            tel.nutec_moving = True
            self.move_alt(-2)
            while(any(self.isMoving())):
                sleep(0.2)
            tel.nutec_moving = False
        return 0
    def move_az(self,x:float,wait=False):
        """
        Description
        -----------
            Perform a movement in the Azimuth by <x> degree.
            negative value will move the telescope to the right

        Parameters
        ----------
        x : FLOAT
            diff. displacement in degree, negative is right.
        wait: BOOL
            If set to true, the function will perfom the move,
            then return the new position
        Returns
        -------
        return: INT 
            0->success, -1 -> failed to move
        position: INT
            return the current position. N.B., only when wait=True
            the position will be valid
        

        """
        tel.rm8_moving = True
        x = int(x*10000.0)
        ret = self.m_RM8.move(x)
        if wait:
            rm8 = self.m_RM8.status()
            #self.hicibas_shm.set_az_moving()
            while(all([rm8>0,rm8<5])):
                sleep(0.2)
                rm8 = self.m_RM8.status()
            #self.hicibas_shm.unset_az_moving()
            p = self.m_RM8.read_position()
            tel.az = p/10000.0
            tel.az_encoder = p
            tel.rm8_moving = False
            return ret,p
        if ret!=0:
            print("Impossible to move the motor",file=stderr)
            tel.rm8_moving = False
            return -1,0
        
        return ret,0
    def move_az_steps(self,x:float,wait=False):
        """
        Description
        -----------
            Perform a movement in the Azimuth by <x> steps.

        Parameters
        ----------
        x : FLOAT
            diff. displacement in steps.
        wait: BOOL
            If set to true, the function will perfom the move,
            then return the new position
        Returns
        -------
        return: INT 
            0->success, -1 -> failed to move
        position: INT
            return the current position. N.B., only when wait=True
            the position will be valid
        

        """
        tel.rm8_moving = True
        x = int(x)
        ret = self.m_RM8.loop(x)
        if wait:
            rm8 = self.m_RM8.status()
            #self.hicibas_shm.set_az_moving()
            while(all([rm8>0,rm8<5])):
                sleep(0.2)
                rm8 = self.m_RM8.status()
            #self.hicibas_shm.unset_az_moving()
            p = self.m_RM8.read_position()
            tel.az = p/10000.0
            tel.az_encoder = p
            tel.rm8_moving = False
            return ret,p
        if ret!=0:
            print("Impossible to move the motor",file=stderr)
            tel.rm8_moving = False
            return -1,0        
        return ret,0
    def isMoving(self):
        """
        Check if both motors are running

        Returns
        -------
        BOOL
            Return True if RM8 is moving.
        BOOL
            Return True if nutec is moving.

        """
        nutec = self.m_nutec.isMoving()
        rm8 = self.m_RM8.status()
        tel.nutec_moving = nutec==1
        tel.rm8_moving = all([rm8>0,rm8<5])
        return all([rm8>0,rm8<5]),nutec==1
    def move_alt_steps(self,y,wait=False):
        """
        Description
        -----------
            Perform a movement in the altitude (elevation) by <y> steps.
            

        Parameters
        ----------
        y : FLOAT
            diff. displacement in steps, negative is upward.
        wait: BOOL
            if set to true, the function will wait for the motor to complete
            the movement before returning the current position.
        Returns
        -------
            The encoder value after the dispacement. The position is
            only meaningful if the wait is set to true.

        """
        tel.nutec_moving = True
        y = int(y)
        self.m_nutec.move_l(y)
        if wait:
            #self.hicibas_shm.set_alt_moving()
            while(self.m_nutec.isMoving()==1):
                sleep(0.2)
            #self.hicibas_shm.unset_alt_moving()
            p,valid = self.m_nutec.get_pos()
            tel.alt = p/10000.0
            tel.alt_encoder = int(p)
            tel.nutec_moving = False
            return p
        p,valid = self.m_nutec.get_pos()
        tel.alt = p/10000.0
        tel.alt_encoder = int(p)
        return p
    def move_alt(self,y,wait=False):
        """
        Description
        -----------
            Perform a movement in the altitude (elevation) by <y> degree.
            Negative value will move the telescope Upward

        Parameters
        ----------
        y : FLOAT
            diff. displacement in degree, negative is upward.
        wait: BOOL
            if set to true, the function will wait for the motor to complete
            the movement before returning the current position.
        Returns
        -------
            The encoder value after the dispacement. The position is
            only meaningful if the wait is set to true.

        """
        tel.nutec_moving = True
        y = int(y*10000.0)
        self.m_nutec.move_l(y)
        if wait:
            #self.hicibas_shm.set_alt_moving()
            while(self.m_nutec.isMoving()==1):
                sleep(0.2)
            #self.hicibas_shm.unset_alt_moving()
            p,valid = self.m_nutec.get_pos()
            tel.alt = p/10000.0
            tel.alt_encoder = int(p)
            tel.nutec_moving = False
            return p
        p,valid = self.m_nutec.get_pos()
        tel.alt = p/10000.0
        tel.alt_encoder = int(p)
        return p
    def move_loop(self,alt:float,az:float):
        """
        This utility function is meant to be use inside a guiding loop.
        

        Parameters
        ----------
        alt : float
            altitude [y] in degree.
        az : float
            azimuth [x] in degree.

        Returns
        -------
        err : INT
            0 -> success
            1 -> problem with RM8 [azimuth]
            3 -> problem with Nutec [altitude]

        """
        err=0
        tel.nutec_moving = True
        tel.rm8_moving = True
        if self.m_RM8.loop(az*10000.0)!=0:
            err = err | 1
        if 'ok' not in self.m_nutec.loop(alt*10000.0):
            err = err | 2
        return err
    def move_steps(self,x:int,y:int,wait=False):
        """
        Description
        -----------
            Move the telescope in x and y position simultaneaously in steps. 
        
        NOTE 
        ----
            It is unclear if the lim. switch can "kill" this function. This method uses
            multitreading which do not have a temrinate methd implemented, therefore the
            only way to make sure the threads are stopped is by exiting the process. 
            Therefore, be carefull in your implementation and make sure the script exit
            if a lim. switch is triggered.
            
        Parameters
        ----------
        x : FLOAT
            Azimuth diff. displacement in steps.
        y : FLOAT
            Altitude diff. displacement in steps.
        wait: BOOL
            If set to true the function will return only when the motor will stop moving
        Returns
        -------
        int
            Current RM8 motor encoder position.
        INT
            Current Nutec motor encoder position.

        """
        
        self.move_alt_steps(y)
        self.move_az_steps(x)
        if not wait:
            tel.nutec_moving = True
            tel.rm8_moving = True
        if wait:
            tel.nutec_moving = True
            tel.rm8_moving = True
            while(any(self.isMoving())):
                sleep(0.5)
                alt,az = self.get_position()

            tel.nutec_moving = False
            tel.rm8_moving = False
            
            return self.get_position()
        alt,az = self.get_position()
        return az,alt    
    def move(self,x:float,y:float,wait=False):
        """
        Description
        -----------
            Move the telescope in x and y position simultaneaously. 
        
        NOTE 
        ----
            It is unclear if the lim. switch can "kill" this function. This method uses
            multitreading which do not have a temrinate methd implemented, therefore the
            only way to make sure the threads are stopped is by exiting the process. 
            Therefore, be carefull in your implementation and make sure the script exit
            if a lim. switch is triggered.
            
        Parameters
        ----------
        x : FLOAT
            Azimuth diff. displacement in degree.
        y : FLOAT
            Altitude diff. displacement in degree.
        wait: BOOL
            If set to true the function will return only when the motor will stop moving
        Returns
        -------
        int
            Current RM8 motor encoder position.
        INT
            Current Nutec motor encoder position.

        """
        
        self.move_alt(y)
        self.move_az(x)
        if not wait:
            tel.nutec_moving = True
            tel.rm8_moving = True
        if wait:
            tel.nutec_moving = True
            tel.rm8_moving = True
            while(any(self.isMoving())):
                sleep(0.5)
                alt,az = self.get_position()

            tel.nutec_moving = False
            tel.rm8_moving = False
            
            return self.get_position()
        alt,az = self.get_position()
        return az,alt
        """
        t_rm8 = ThreadWithResult(target=self.m_RM8.loop, args=(int(x),)) #RM-XX move command (for a loop)
        t_nutec = ThreadWithResult(target=self.m_nutec.loop, args=(int(y),)) #Nutec move command (for a loop)
        t_rm8.daemon = True#we absolutely need this 
        t_nutec.daemon = True#we absolutely need this
        t_rm8.log_thread_status = False
        t_nutec.log_thread_status = False
            # starting thread 1
        t_rm8.start()
            # starting thread 2
        t_nutec.start()

        # wait until thread 1 is completely executed
        t_rm8.join()
        # wait until thread 2 is completely executed
        t_nutec.join()
        t_nutec.result      
        position_rm8 = self.m_RM8.read_position()
        return position_rm8,t_nutec.result 
        """

def help():
    print("")

if __name__ == "__main__":

    az = 0
    alt = 0
    for arg in argv:
        if '--az' in arg:
            az = float(arg.replace('--az=',''))
        if '--alt' in arg:
            alt = float(arg.replace('--alt=',''))
    if az==0 and alt==0:
        exit(0)
        
    if '--override-lim-switch' in argv:
        
        with moteurs() as tcs:
            p = tcs.get_position()
            print(f"Az: {p[0]}, Alt: {p[1]}")
            print("moving...")
            tcs.move(az,alt,wait=True)
            print("done!")
            p = tcs.get_position()
            print(f"Az: {p[0]}, Alt: {p[1]}")
    
    conn = "Ethernet"    
    conn = get_string("/opt/HiCIBaS/config/network.conf","LJMODE",default=conn)
    
                    
    with Labjack_lim(connection=conn) as lim:
        h,zero = lim.anySwitchActivated()
        if h:
            print("Some lim. switch are activated",file=stderr)
            exit(0)
        def move(az,alt):
            with moteurs() as m:
                m.move(az,alt,wait=True)
            return 0
        lim_switch,return_code = lim.execute_with_limswitch(move,args=(az,alt))
        if lim_switch!="":
            exit(0)
        else:
            print("%s lim. switch activated in the process",file=stderr)
            exit(1)