import os
from ids_peak import ids_peak
from ids_peak_ipl import ids_peak_ipl
import numpy as np
from re import match
from functools import reduce
from astropy.io import fits

os.environ["PEAK_PATH"] = "/usr/bin"
os.environ["LD_LIBRARY_PATH"] = f"{os.environ['PEAK_PATH']}/../lib:$LD_LIBRARY_PATH"
os.environ[
    "GENICAM_GENTL32_PATH"
] = f"{os.environ['GENICAM_GENTL32_PATH'] if 'GENICAM_GENTL32_PATH' in os.environ else ''}:{os.environ['PEAK_PATH']}/../lib/ids/cti"
os.environ[
    "GENICAM_GENTL64_PATH"
] = f"{os.environ['GENICAM_GENTL64_PATH'] if 'GENICAM_GENTL64_PATH' in os.environ else ''}:{os.environ['PEAK_PATH']}/../lib/ids/cti"


class lrio_peak_error(Exception):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)


def _max_pixel_format(xx, yy):
    if xx is None:
        return
    if (match("(Mono).*", xx) is not None) and (match("(Mono).*", yy) is None):
        return yy
    elif int(match(r"Mono(\d*)p?", xx)[1]) < int(match(r"Mono(\d*)p?", yy)[1]):
        return yy
    elif int(match(r"Mono(\d*)p?", xx)[1]) == int(match(r"Mono(\d*)p?", yy)[1]):
        return xx if "p" in match(r"Mono(\d*p?)", yy)[1] else yy
    else:
        return xx
def _get_peak_camera_list():
    try:
        ids_peak.Library.Initialize()
        device_manager = ids_peak.DeviceManager.Instance()
        device_manager.Update()
        camera_list = []
        for i, device in enumerate(device_manager.Devices()):
            info_dict = {}
            info_dict["InUse"] = not device.IsOpenable()
            info_dict["SerialNumber"] = str(device.SerialNumber())
            info_dict["ModelName"] = device.ModelName()
            camera_list.append(info_dict)
    except Exception as e:
        raise e
    finally:
        ids_peak.Library.Close()
    return camera_list

class lrio_peak:
    def disconnect_if_error(f):
        """Decorator that disconnect the camera if the method fails"""

        def func(*args, **kwargs):
            try:
                return f(*args, **kwargs)
            except Exception as e:
                ids_peak.Library.Close()
                print("Successfull emergency disconnect")
                raise e from None

        return func

    @disconnect_if_error
    def __init__(self, SerialNumber=None):
        ids_peak.Library.Initialize()
        device_manager = ids_peak.DeviceManager.Instance()
        device_manager.Update()
        if device_manager.Devices().size() == 0:
            raise lrio_peak_error("No camera found")
        if all([x.IsOpenable() for x in device_manager.Devices()]) is False:
            raise lrio_peak_error("No camera available")
        if SerialNumber is None:
            self.handle = device_manager.Devices()[
                [x.IsOpenable() for x in device_manager.Devices()].index(True)
            ].OpenDevice(ids_peak.DeviceAccessType_Control)
        else:
            SerialNumber = str(SerialNumber)
            if SerialNumber not in [x.SerialNumber() for x in device_manager.Devices()]:
                raise lrio_peak_error("Camera not found")
            device_index = [x.SerialNumber() for x in device_manager.Devices()].index(
                SerialNumber
            )
            if device_manager.Devices()[device_index].IsOpenable() is False:
                raise lrio_peak_error("Camera not available")
            self.handle = device_manager.Devices()[device_index].OpenDevice(
                ids_peak.DeviceAccessType_Control
            )

        self.__node_map_remote_device = self.handle.RemoteDevice().NodeMaps()[0]
        self.__min_exposure_time = self.__node_map_remote_device.FindNode(
            "ExposureTime"
        ).Minimum()
        self.__max_exposure_time = self.__node_map_remote_device.FindNode(
            "ExposureTime"
        ).Maximum()
        self.__min_fps = self.__node_map_remote_device.FindNode(
            "AcquisitionFrameRate"
        ).Minimum()
        self.__max_fps = self.__node_map_remote_device.FindNode(
            "AcquisitionFrameRate"
        ).Maximum()
        self.__min_gain = self.__node_map_remote_device.FindNode("Gain").Minimum()
        self.__max_gain = self.__node_map_remote_device.FindNode("Gain").Maximum()
        self.__min_gamma = self.__node_map_remote_device.FindNode("Gamma").Minimum()
        self.__max_gamma = self.__node_map_remote_device.FindNode("Gamma").Maximum()

        if self.handle.DataStreams().empty():
            raise lrio_peak_error("No datastream found")
        else:
            self.__data_stream = self.handle.DataStreams()[0].OpenDataStream()
        self.__set_max_roi()
        try:
            self.__node_map_remote_device.FindNode("UserSetSelector").SetCurrentEntry(
                "Default"
            )
            self.__node_map_remote_device.FindNode("UserSetLoad").Execute()
            self.__node_map_remote_device.FindNode("UserSetLoad").WaitUntilDone()
        except ids_peak.Exception:
            pass
        __allEntries = self.__node_map_remote_device.FindNode("PixelFormat").Entries()
        self.__availablePixelFormat = []
        for entry in __allEntries:
            if (
                entry.AccessStatus() != ids_peak.NodeAccessStatus_NotAvailable
                and entry.AccessStatus() != ids_peak.NodeAccessStatus_NotImplemented
            ):
                self.__availablePixelFormat.append(entry.SymbolicValue())
        self.__max_pixel_format = reduce(
            _max_pixel_format, (self.__availablePixelFormat)
        )
        self.__node_map_remote_device.FindNode("PixelFormat").SetCurrentEntry(
            self.__max_pixel_format
        )
        self.__set_allocated_memory()
        self.__acquisition_running = False
    def __enter__(self):
        return self
    def __exit__(self,a,b,c):
        try:
            self.stop_acquisition()
        except:
            pass
        try:
            self.close()
        except:
            pass
        return
    def save_to_fits(self,fname,overwrite=True):
        """
        Grab an image and save it to FITS file. 

        Parameters
        ----------
        fname : STR
            Filename.
        overwrite : BOOL, optional
            Overwrite if file already exist. The default is True.

        Returns
        -------
        None.

        """
        data = self.get_image()
        fits.PrimaryHDU(data=data).writeto(fname,overwrite=overwrite)
    def start_acquisition(self):
        if self.__acquisition_running:
            return None
        self.__set_allocated_memory()
        self.__data_stream.StartAcquisition(
            ids_peak.AcquisitionStartMode_Default, ids_peak.DataStream.INFINITE_NUMBER
        )
        self.__node_map_remote_device.FindNode("TLParamsLocked").SetValue(1)
        self.__node_map_remote_device.FindNode("AcquisitionStart").Execute()
        self.__node_map_remote_device.FindNode("AcquisitionStart").WaitUntilDone()
        self.__acquisition_running = True

    def stop_acquisition(self):
        if self.__acquisition_running is False:
            return None
        self.__node_map_remote_device.FindNode("AcquisitionStop").Execute()
        self.__node_map_remote_device.FindNode("AcquisitionStop").WaitUntilDone()
        self.__data_stream.StopAcquisition(ids_peak.AcquisitionStopMode_Default)
        self.__acquisition_running = False
        self.__node_map_remote_device.FindNode("TLParamsLocked").SetValue(0)

    def close(self):
        if self.__data_stream is not None:
            try:
                self.__data_stream.Flush(ids_peak.DataStreamFlushMode_DiscardAll)
                self.__data_stream.KillWait()
                for buffer in self.__data_stream.AnnouncedBuffers():
                    self.__data_stream.RevokeBuffer(buffer)
            except Exception as e:
                raise e
        ids_peak.Library.Close()

    def get_image(self):
        buffer = self.__data_stream.WaitForFinishedBuffer(5000)
        ipl_image = ids_peak_ipl.Image_CreateFromSizeAndBuffer(
            buffer.PixelFormat(),
            buffer.BasePtr(),
            buffer.Size(),
            buffer.Width(),
            buffer.Height(),
        )
        buffer_pixel_format = buffer.PixelFormat()
        buffer_shape = (buffer.Height(), buffer.Width())
        self.__data_stream.QueueBuffer(buffer)
        if buffer_pixel_format == int(ids_peak_ipl.PixelFormatName_Mono12):
            return ipl_image.get_numpy_1D_16().reshape(buffer_shape)
        elif buffer_pixel_format == int(ids_peak_ipl.PixelFormatName_Mono8):
            return ipl_image.get_numpy_1D().reshape(buffer_shape)
        else:
            raise lrio_peak_error("Pixel format not supported")

    def __set_max_roi(self):
        x_min = self.__node_map_remote_device.FindNode("OffsetX").Minimum()
        y_min = self.__node_map_remote_device.FindNode("OffsetY").Minimum()
        w_min = self.__node_map_remote_device.FindNode("Width").Minimum()
        h_min = self.__node_map_remote_device.FindNode("Height").Minimum()
        self.__node_map_remote_device.FindNode("OffsetX").SetValue(x_min)
        self.__node_map_remote_device.FindNode("OffsetY").SetValue(y_min)
        self.__node_map_remote_device.FindNode("Width").SetValue(w_min)
        self.__node_map_remote_device.FindNode("Height").SetValue(h_min)
        x_max = self.__node_map_remote_device.FindNode("OffsetX").Maximum()
        y_max = self.__node_map_remote_device.FindNode("OffsetY").Maximum()
        w_max = self.__node_map_remote_device.FindNode("Width").Maximum()
        h_max = self.__node_map_remote_device.FindNode("Height").Maximum()
        self.__node_map_remote_device.FindNode("OffsetX").SetValue(x_min)
        self.__node_map_remote_device.FindNode("OffsetY").SetValue(y_min)
        self.__node_map_remote_device.FindNode("Width").SetValue(w_max)
        self.__node_map_remote_device.FindNode("Height").SetValue(h_max)

    def __set_allocated_memory(self):
        self.__data_stream.Flush(ids_peak.DataStreamFlushMode_DiscardAll)
        for buffer in self.__data_stream.AnnouncedBuffers():
            self.__data_stream.RevokeBuffer(buffer)
        payload_size = self.__node_map_remote_device.FindNode("PayloadSize").Value()
        num_buffers_min_required = self.__data_stream.NumBuffersAnnouncedMinRequired()
        for count in range(num_buffers_min_required):
            buffer = self.__data_stream.AllocAndAnnounceBuffer(payload_size)
            self.__data_stream.QueueBuffer(buffer)

    @property
    def exposure_time(self):
        """The exposure_time property."""
        return None

    @exposure_time.setter
    @disconnect_if_error
    def exposure_time(self, value):
        if value < self.__min_exposure_time or value > self.__max_exposure_time:
            raise lrio_peak_error(
                f"Exposure time out of range: {value} (min: {self.__min_exposure_time}, max: {self.__max_exposure_time})"
            )
        self.__node_map_remote_device.FindNode("ExposureTime").SetValue(value)

    @exposure_time.getter
    @disconnect_if_error
    def exposure_time(self):
        return self.__node_map_remote_device.FindNode("ExposureTime").Value()

    @property
    def fps(self):
        """The fps property."""
        return None

    @fps.setter
    @disconnect_if_error
    def fps(self, value):
        if value < self.__min_fps or value > self.__max_fps:
            raise lrio_peak_error(
                f"FPS time out of range: {value} (min: {self.__min_fps}, max: {self.__max_fps})"
            )
        self.__node_map_remote_device.FindNode("AcquisitionFrameRate").SetValue(value)

    @fps.getter
    @disconnect_if_error
    def fps(self):
        return self.__node_map_remote_device.FindNode("AcquisitionFrameRate").Value()

    @property
    def gain(self):
        """The gain property."""
        return None

    @gain.setter
    @disconnect_if_error
    def gain(self, value):
        if value < self.__min_gain or value > self.__max_gain:
            raise lrio_peak_error(
                f"Gain time out of range: {value} (min: {self.__min_gain}, max: {self.__max_gain})"
            )
        self.__node_map_remote_device.FindNode("GainSelector").SetCurrentEntry(
            "AnalogAll"
        )
        self.__node_map_remote_device.FindNode("Gain").SetValue(value)

    @gain.getter
    @disconnect_if_error
    def gain(self):
        return self.__node_map_remote_device.FindNode("Gain").Value()

    @property
    def gamma(self):
        """The gamma property."""
        return None

    @gamma.setter
    @disconnect_if_error
    def gamma(self, value):
        if value < self.__min_gamma or value > self.__max_gamma:
            raise lrio_peak_error(
                f"Ggamma time out of range: {value} (min: {self.__min_gamma}, max: {self.__max_gamma})"
            )
        self.__node_map_remote_device.FindNode("LUTEnable").SetValue(False)
        self.__node_map_remote_device.FindNode("Gamma").SetValue(value)

    @gamma.getter
    @disconnect_if_error
    def gamma(self):
        return self.__node_map_remote_device.FindNode("Gamma").Value()

if 'main' in __name__:
    #open the camera, change the FPS and gain, then save
    #the data to disk
    
    with lrio_peak() as SH:#connect to hardware
        SH.fps = 50#set the FPS
        SH.gain = 1#set the gain
        SH.save_to_fits("/home/hicibas-clone/tmp.fits")#save data to disk
    #SH "automatically" disconnect
    #display the image
    if os.path.isfile("/home/hicibas-clone/tmp.fits"):
        os.popen("ds9 -zscale -zoom to fit /home/hicibas-clone/tmp.fits &").read()
        
    
        


