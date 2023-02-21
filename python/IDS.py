#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Feb  7 08:24:20 2023

@author: espressjo
"""
from ctypes import c_int,c_uint
from datetime import datetime
from pyueye import ueye
from astropy.io import fits
from sys import stdout
import numpy as np
from os.path import isfile

from Hlog import LHiCIBaS
log = LHiCIBaS(__file__)


class ids():
    """
    Description
    ===========
    
    Methods
    -------
        start_video -> Start a continuous stream of images\n
        stop_video -> Stop the acquisition\n
        set_memory -> Must be called after initialization of AOI and ADC\n
        get_data -> get an numpy array with the last images in buffer\n
        get_AOI -> get the AOI structure\n
        set_AOI -> set the AOI structure\n
        set_adc -> Set the ADC resolution\n
        get_gain_boost -> Get the status of the gain boost\n
        get_gain -> get the hardware gain (0-100)\n
        set_gain -> set the hardware gain (0-100)\n
        set_gain_boost -> Set the gain boost\n
        unset_gain_boost -> unset the gain boost\n
        set_max_expt -> Set the maximum exposure time given the FPS\n
        set_abs_expt -> Set the exp. time and will adjust the FPS accordingly\n
        set_expt -> Set the exposure time\n
        get_expt -> Get the exp. time\n
        set_fps -> Set the FPS\n
        get_fps -> Return the FPS\n
        get_fits -> Will get data and save a FITS file\n
        
    NOTES
    -----
        You can either call start_video() before a series of get_data in loop
        or not call it. If you choose to call it, get_data will return immediatly
        no matter the exposure time. You will end up with several of the same images
        Otherwise by not calling start_video, the get_data will take ~ the exp time
        before returning.
    
    Example
    -------
    .. highlight:: python
    .. code-block:: python
    
        with ids("4103216958") as cam:
            cam.set_adc(12)
            cam.set_AOI(0, 0, 500, 301)
            cam.set_memory()  
            cam.set_abs_expt(50)
            cam.set_gain(50)
            cam.set_gain_boost()
            cam.get_fits("test.fits")
    
    """
    def __init__(self,serno=""):
        self.handle = None
        self.width = ueye.c_int(0)
        self.height = ueye.c_int(0)
        self.model = ""
        self.serno = serno
        self.sInfo = ueye.SENSORINFO()
        self.cInfo = ueye.CAMINFO()
        self.is_connected = False
        self.pcImageMemory = ueye.c_mem_p()
        self.MemID = ueye.int()
        self.rectAOI = ueye.IS_RECT()
        self.pitch = ueye.INT()
        self.nBitsPerPixel = ueye.INT(12)    
        self.channels = 1                   
        self.m_nColorMode = ueye.IS_CM_MONO12		
        self.bytes_per_pixel = 2
    def make_sure_video_is_off(func):
        def wrapper(self,*args):
            _open = False
            if ueye.is_CaptureVideo(self.handle,ueye.IS_GET_LIVE):
                _open = True
                self.stop_video()
            ret = func(self,*args)
            if _open:
                self.start_video()
            return ret
        return wrapper    
    def make_sure_video_is_on(func):
        def wrapper(self,*args):
            close = False
            if not ueye.is_CaptureVideo(self.handle,ueye.IS_GET_LIVE):
                close = True
                self.start_video()
            ret = func(self,*args)
            if close:
                self.stop_video()
            return ret
        return wrapper
    def stop_video(self):
        if ueye.IS_SUCCESS!=ueye.is_StopLiveVideo(self.handle, ueye.IS_FORCE_VIDEO_STOP):
            print("Failed to stop video")
            log.warning("Failed to stop video")
            return -1
        return 0
    def start_video(self):

        if ueye.IS_SUCCESS!=ueye.is_InquireImageMem(self.handle, self.pcImageMemory, self.MemID, self.width, self.height, self.nBitsPerPixel, self.pitch):
            print("is_InquireImageMem ERROR",file=stdout)
            log.critical("is_InquireImageMem ERROR")
            return -1
        if ueye.is_CaptureVideo(self.handle,ueye.IS_GET_LIVE):
            return 0
        if ueye.IS_SUCCESS!=ueye.is_CaptureVideo(self.handle, ueye.IS_WAIT):
            log.critical("is_CaptureVideo ERROR")
            print("is_CaptureVideo ERROR",file=stdout)
            return -1
        return 0
    def set_memory(self):
        """
        Set the memory for the images based on the AOI, ADC and colormode

        Raises
        ------
        Exception
            DESCRIPTION.

        Returns
        -------
        None.

        """
        if ueye.IS_SUCCESS != self.get_AOI():
            log.critical("Unable to get the current AOI")
            raise Exception("Unable to get the current AOI")
        if ueye.IS_SUCCESS != ueye.is_AllocImageMem(self.handle, self.width, self.height, self.nBitsPerPixel, self.pcImageMemory, self.MemID):
            log.critical("Unable to set the memory")
            raise Exception("Unable to set the memory")
        if ueye.IS_SUCCESS != ueye.is_SetImageMem(self.handle, self.pcImageMemory, self.MemID):
            log.critical("is_SetImageMem ERROR")
            raise Exception("is_SetImageMem ERROR")
        if ueye.IS_SUCCESS != ueye.is_SetColorMode(self.handle, self.m_nColorMode):
            log.critical("Unable to set color mode")
            raise Exception("Unable to set color mode")
    @make_sure_video_is_on
    def get_data(self):
        array = ueye.get_data(self.pcImageMemory, self.width, self.height, self.nBitsPerPixel, self.pitch, copy=True)
        if self.bytes_per_pixel!=1:
            array = array.view(np.uint16)
        return np.reshape(array,(self.height.value, self.width.value))
    def get_AOI(self):
        """
        Update the rectAOI, width and height with the values
        set in the camera.

        Returns
        -------
        ret : INT
            0 if successfull..

        """
        ret = ueye.is_AOI(self.handle, ueye.IS_AOI_IMAGE_GET_AOI, self.rectAOI, ueye.sizeof(self.rectAOI))
        self.width = self.rectAOI.s32Width
        self.height = self.rectAOI.s32Height
        return ret
    def get_trigger(self):
        print("Trigger: ",ueye.is_SetExternalTrigger(self.handle, ueye.IS_GET_EXTERNALTRIGGER))
    def set_AOI(self,x,y,width,height):
        """
        Set an Area Of Interest. You will need to reallocate the memory once 
        this function is called. 
        
        Note
        ----
        width must be a multiple of 8
        height must be a multiple of 2
        If the are not they will automatically be set to the nearest width and height

        Parameters
        ----------
        x : INT
            x-position (lower left pixel position)
        y : INT
            y-position (lower left pixel position).
        width : INT
            DESCRIPTION.
        height : INT
            DESCRIPTION.

        Returns
        -------
        ret : INT
            0 if successfull.

        """
        if width%8!=0:
            multi = int(width/8.)
            width = multi*8
        if height%2!=0:
            multi = int(height/2.)
            height = multi*2
        self.rectAOI.s32X     = ueye.c_int(x)
        self.rectAOI.s32Y     = ueye.c_int(y)
        self.rectAOI.s32Width = ueye.c_int(width)
        self.rectAOI.s32Height = ueye.c_int(height)
        ret = ueye.is_AOI(self.handle, ueye.IS_AOI_IMAGE_SET_AOI, self.rectAOI, ueye.sizeof(self.rectAOI))
        if ret!=ueye.IS_SUCCESS:
            print("Unable to set AOI",file=stdout)
            return ret
        #update the rectAOI, width and height value
        self.get_AOI()
        self.width = self.rectAOI.s32Width
        self.height = self.rectAOI.s32Height
        return 0
    def set_adc(self,bits=12):
        """
        Sets the value for the color mode of the camera. 

        Parameters
        ----------
        bits : INT, optional
            ADC bits 8, 10 and 12-bits available. The default is 12.

        Returns
        -------
        None.

        """
        if bits==8:
            self.nBitsPerPixel = ueye.INT(8)
            self.m_nColorMode = ueye.IS_CM_MONO8
            self.bytes_per_pixel = 1
        elif bits==10:
            self.nBitsPerPixel = ueye.INT(10)
            self.m_nColorMode = ueye.IS_CM_MONO10
            self.bytes_per_pixel = 2
        elif bits==12:
            self.nBitsPerPixel = ueye.INT(12)
            self.m_nColorMode = ueye.IS_CM_MONO12
            self.bytes_per_pixel = 2
        else:
            log.warning("Bits value out of range, we'll default to 12-bits")
            print("Bits value out of range, we'll default to 12-bits",file=stdout)
            self.nBitsPerPixel = ueye.INT(12)
            self.m_nColorMode = ueye.IS_CM_MONO12
            self.bytes_per_pixel = 2
    def free_memory(self):
        if ueye.is_InquireImageMem(self.handle,self.pcImageMemory,self.MemID,None,None,None,None)!=ueye.IS_SUCCESS:
            return
        if ueye.IS_SUCCESS!=ueye.is_FreeImageMem(self.handle, self.pcImageMemory, self.MemID):
            print("Unable to free memory",file=stdout)
            log.critical("Unable to free memory")
            return
    def get_gain_boost(self):
        if ueye.IS_SET_GAINBOOST_ON ==  ueye.is_SetGainBoost(self.handle,ueye.IS_GET_GAINBOOST):
            return 1
        elif ueye.IS_SET_GAINBOOST_OFF ==  ueye.is_SetGainBoost(self.handle,ueye.IS_GET_GAINBOOST):
            return 0
        else:
            return -1
    @make_sure_video_is_off
    def unset_gain_boost(self):
        return ueye.is_SetGainBoost(self.handle,ueye.IS_SET_GAINBOOST_OFF)
    @make_sure_video_is_off
    def set_gain_boost(self):
        return ueye.is_SetGainBoost(self.handle,ueye.IS_SET_GAINBOOST_ON)
    def get_gain(self):
        gain = ueye.is_SetHardwareGain(self.handle,ueye.IS_GET_MASTER_GAIN,ueye.IS_IGNORE_PARAMETER,ueye.IS_IGNORE_PARAMETER,ueye.IS_IGNORE_PARAMETER)
        return gain
    @make_sure_video_is_off
    def set_gain(self,gain):
        gain = ueye.is_SetHardwareGain(self.handle,gain,ueye.IS_IGNORE_PARAMETER,ueye.IS_IGNORE_PARAMETER,ueye.IS_IGNORE_PARAMETER)
        return self.get_gain()
    @make_sure_video_is_off
    def set_max_expt(self):
        exp = ueye.double(0)#set to 1/fps
        if ueye.IS_SUCCESS!=ueye.is_Exposure(self.handle,ueye.IS_EXPOSURE_CMD_SET_EXPOSURE,exp,ueye.sizeof(exp)):
            return -1
        return self.get_expt()
    @make_sure_video_is_off
    def set_abs_expt(self,exp_ms):
        
            
        fps = 1.0/(exp_ms/1000.0)
        self.set_fps(fps)
        if exp_ms>1000:
            self.set_expt(exp_ms)
        else:
            self.set_max_expt()
    @make_sure_video_is_off
    def set_expt(self,exp_ms):
        exp = ueye.double(exp_ms)
        if ueye.IS_SUCCESS!=ueye.is_Exposure(self.handle,ueye.IS_EXPOSURE_CMD_SET_EXPOSURE,exp,ueye.sizeof(exp)):
            return -1
        return self.get_expt()
    def get_expt(self):
        exp = ueye.double()
        if ueye.IS_SUCCESS!=ueye.is_Exposure(self.handle,ueye.IS_EXPOSURE_CMD_GET_EXPOSURE,exp,ueye.sizeof(exp)):
            return -1 
        return exp.value
    @make_sure_video_is_off
    def set_fps(self,fps):
        fps = ueye.double(fps)
        newfps = ueye.double()
        if ueye.IS_SUCCESS!=ueye.is_SetFrameRate(self.handle,fps,newfps):
            return -1
        return newfps
    def get_fps(self):
        fps = ueye.double()
        if ueye.IS_SUCCESS!=ueye.is_SetFrameRate(self.handle,ueye.IS_GET_FRAMERATE,fps):
            return -1
        return fps.value
    def get_fits(self,fname):
        im = self.get_data()   
        fps = self.get_fps()
        gain = self.get_gain()
        boost = self.get_gain_boost()
        expt = self.get_expt()
        fmt = "%Y-%m-%dT%H:%M:%S"
        dt = datetime.now().strftime(fmt)
        ADC = self.nBitsPerPixel.value 
        serial = self.cInfo.SerNo.decode()
        model = self.sInfo.strSensorName.decode()
        hdu = fits.PrimaryHDU(data=im)
        hdu.header["FPS"] = (fps,"Frame per second")
        hdu.header["DATE"] = (dt,"Creation date")
        hdu.header["GAIN"] = (gain,"Hardware Gain")
        hdu.header["BOOST"] = ( True if boost==1 else False,"Gain boost Enabled")
        hdu.header["EXPT"] = (expt,"Exposure time (milisecond)")
        hdu.header["ADC"] = (ADC,"ADC (8-bits, 10-bits, 12-bits)")
        saturation = 2**ADC - 1 
        hdu.header["SAT"] = (saturation,"Max. Value")
        hdu.header["SERIAL"] = (serial,"Serial Number")
        hdu.header["MODEL"] = (model,"Sensor model")
        hdu.writeto(fname,overwrite=True)
        if isfile(fname):
            return fname
        else:
            return ""
    def __enter__(self):
        self.connect()
        return self 
    def __exit__(self,a,b,c):
        self.free_memory()
        self.disconnect()
    def connect(self):
        """
        Connect to a specific camera with the given serial number. 
        
        Note
        -----
        You should use the __enter__ / __exit__ methods.

        Parameters
        ----------
        serno : STR
            IDS Serial Number

        Raises
        ------
        Exception
            Camera Already In Use

        Returns
        -------
        None.

        """
        cam_list,ret = self.get_cam_list()
        _cam_id = None
        if ret!=0:
            return 
        for _uic in cam_list:
            if self.serno in _uic.SerNo.decode():
                if _uic.dwInUse.value!=0:
                    log.critical("Camera Already In Use")
                    raise Exception("Camera Already In Use")
                _cam_id = _uic.dwCameraID
        if _cam_id!=None:
            
            self.handle = ueye.HIDS(_cam_id)
            if ueye.IS_SUCCESS != ueye.is_InitCamera(self.handle, None):
                log.critical("Unable to initialize the camera")
                raise Exception("Unable to initialize the camera")
            self.model = _uic.Model.decode()
            self.serno = _uic.SerNo.decode()
            self.is_connected = True
            if ueye.IS_SUCCESS != ueye.is_GetCameraInfo(self.handle, self.cInfo):
                log.critical("Unable to fetch camera info")
                raise Exception("Unable to fetch camera info")
            if ueye.IS_SUCCESS != ueye.is_GetSensorInfo(self.handle, self.sInfo):
                log.critical("Unable to fetch detector info")
                raise Exception("Unable to fetch detector info")
            if ueye.IS_SUCCESS != ueye.is_ResetToDefault( self.handle):
                log.critical("Unable to reset the camera")
                raise Exception("Unable to reset the camera")
            if ueye.IS_SUCCESS != ueye.is_SetDisplayMode(self.handle, ueye.IS_SET_DM_DIB):
                log.critical("Unable to set the display mode to DIB")
                raise Exception("Unable to set the display mode to DIB")
        else:
            log.warning("Camera not found")
            print("Camera not found",file=stdout)
            self.is_connected = False
    def disconnect(self):
        if ueye.is_CaptureVideo(self.handle,ueye.IS_GET_LIVE):
            self.stop_video()
        self.free_memory()
        ret = None
        if self.handle != None:
            ret = ueye.is_ExitCamera(self.handle)
        if ret == ueye.IS_SUCCESS:
            self.handle = None
            self.model = ""
            self.serno = ""
            self.is_connected = False
        return
    def get_cam_list(self):
        """
        Return a 

        Returns
        -------
        cam_struct_list.uci : list
            Array of UEYE_CAMERA_LIST structure
        Ret: INT
            0-> successful, -1 -> failed.

        """
        nb_cam,ret = self.get_number_of_camera()
        #declare a list of cam info
        cam_struct_list = ueye.UEYE_CAMERA_LIST((ueye.UEYE_CAMERA_INFO * nb_cam))
        cam_struct_list.dwCount = ueye.c_uint(nb_cam)
        if ueye.IS_SUCCESS!= ueye.is_GetCameraList(cam_struct_list):
            return list(cam_struct_list.uci),-1
        return list(cam_struct_list.uci),0
    def get_number_of_camera(self):
        """
        Return the number of camera found on the network.

        Returns
        -------
        Nb of Cam. : INT
            Number of cam found
        Ret: INT
            0-> successful, -1 -> failed.

        """
        nb_cam = c_int()
        if ueye.IS_SUCCESS != ueye.is_GetNumberOfCameras(nb_cam):
            return 0,-1
        return nb_cam.value,0
    def set_software_trigger(self):
        
        print("Set Trigger: ",ueye.is_SetExternalTrigger(self.handle,ueye.IS_SET_TRIGGER_SOFTWARE))
    def unset_software_trigger(self):
        print("unset trigger: ",ueye.is_SetExternalTrigger(self.handle,ueye.IS_SET_TRIGGER_OFF))
    def get_data_triggered(self):
        print("start video: ",self.start_video())
        return self.get_data()
if '__main__'in __name__:
    # from matplotlib import pyplot as plt
    # def hist(data,title=''):
    #     f,ax = plt.subplots()
    #     from astropy.stats import sigma_clipped_stats as sc
    #     mean,_,std = sc(data,sigma=5)
    #     data = data[(data>(mean-5*std)) & (data<(mean+5*std))]
    #     ax.hist(data.ravel(),bins=100)
    #     ax.set(title=title)
    #     return f,ax
    import time 
    with ids("4103216958") as cam:
        cam.set_adc(12)
        cam.set_AOI(0, 0, 500, 301)
        cam.set_memory()  
        cam.set_abs_expt(50)
        cam.set_gain(50)
        cam.set_gain_boost()
        Cube = [];
        #cam.set_software_trigger()
        #cam.start_video()
        cam.get_trigger()
        for i in range(10):
            print(f"Image {i+1} taken")
            t1 = time.time_ns()
            Cube.append(cam.get_data())
            t2 = time.time_ns()
            delta = (t2-t1)/(10**9)
            print("Delta: ",delta)
    fits.PrimaryHDU(data=Cube).writeto('cube.fits',overwrite=True)
        
    
    