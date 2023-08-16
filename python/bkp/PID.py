import numpy as np


class PID:
    def __init__(self, kp=1, kd=0, ki=0, base_output=0, base_set_point=0):
        self.__kp = kp
        self.__kd = kd
        self.__ki = ki
        self.__previous_error = 0
        self.__previous_output = base_output
        self.__base_output = base_output
        self.__set_point = base_set_point
        self.__integral = 0
        self.previous_output_diff = np.zeros(10)

    @property
    def base_output(self):
        return self.__base_output

    @property
    def previous_output(self):
        return self.__previous_output

    def update_output(self, output):
        self.__previous_output = output
        self.previous_output_diff = np.roll(self.previous_output_diff,-1,axis=0)
        self.previous_output_diff[-1] = output
        return self.__previous_output

    def reset_previous_output(self, reset_value):
        self.__previous_output = reset_value

    @property
    def kp(self):
        return self.__kp

    @kp.setter
    def kp(self, value):
        if np.size(value) == 1:
            self.__kp = value
        else:
            raise TypeError('Gain setting needs to be a float')

    @property
    def kd(self):
        return self.__kd

    @kd.setter
    def kd(self, value):
        if np.size(value) == 1:
            self.__kd = value
        else:
            raise TypeError('Gain setting needs to be a float')

    @property
    def ki(self):
        return self.__ki

    @ki.setter
    def ki(self, value):
        if np.size(value) == 1:
            self.__ki = value
        else:
            raise TypeError('Gain setting needs to be a float')

    @property
    def set_point(self):
        return self.__set_point

    @set_point.setter
    def set_point(self, value):
        if np.size(value) <= 2:
            self.__set_point = np.array(value)
        else:
            raise TypeError('Set point needs to be a float or size two')

    def log_error(self, error, dt):

        if np.sign(self.__previous_error) != np.sign(error):
            self.__integral == 0
        else:
            self.__integral = self.__integral*0.5 + (error) * dt
        self.__previous_error = error

    def output(self, error, dt):
        return (self.kp*error +
                self.kd*(error - self.__previous_error)/dt +
                self.ki*self.__integral
                )

    def step(self, error, dt=1): # The important function. The only one directly used in moteurs.py
        output_diff = self.output(error, dt)
        self.log_error(error, dt)
        return self.update_output(output_diff)

    def sensor_gain(self, error):
        return 1*error

    @property
    def previous_output(self):
        return self.__previous_output

    @property
    def previous_error(self):
        return self.__previous_error

    def reset(self):
        self.__previous_error = self.__previous_error*0
        self.__integral = self.__integral *0
        self.reset_previous_output(self.previous_output*0)
        self.previous_output_diff = self.previous_output_diff*0


class Adaptive_gain_PID(PID):
    def __init__(self, learning_time=0.0005, lag=4, **kwargs):
        super().__init__(**kwargs)
        self.previous_output_diff = np.zeros(lag+1)
        self.__learning_time = learning_time
        if lag < 1:
            raise AttributeError("Lag must be bigger than 1")
        else:
            self.__lag = np.int(lag)

    def step(self, signal, dt=1):
        #self.ki += np.mean(self.__learning_time *
        #                   (self.previous_error-self.get_error(signal)) *
        #                   self.previous_output_diff[-self.__lag])
        # self.kp += np.mean(self.__learning_time *(self.previous_error-self.get_error(signal))*self.previous_output_diff[-self.__lag])
        return super().step(signal, dt)
