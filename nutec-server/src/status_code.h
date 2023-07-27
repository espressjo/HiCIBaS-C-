#ifndef STATUS_CODE
#define STATUS_CODE

//Trajectory Status Register
#define R0XC9(x) \
    ((x) == 0  ? "Reserved for future use." \
    : (x) == 1 ? "Reserved for future use." \
    : (x) == 2 ? "Reserved for future use." \
    : (x) == 3 ? "Reserved for future use." \
    : (x) == 4 ? "Reserved for future use." \
    : (x) == 5 ? "Reserved for future use." \
    : (x) == 6 ? "Reserved for future use." \
    : (x) == 7 ? "Reserved for future use." \
    : (x) == 8 ? "Reserved for future use." \
    : (x) == 9 ? "Cam table underflow." \
    : (x) == 10 ? "Reserved for future use" \
    : (x) == 11 ? "Homing error. If set, an error occurred in the last home attempt. Cleared by a home\
    command." \
    : (x) == 12 ? "Referenced. Set when a homing command has been successfully executed. Cleared by\
    a home command." \
    : (x) == 13 ? "Homing. If set, the drive is running a home command." \
    : (x) == 14 ? "Set when a move is aborted. Cleared at the start of the next move." \
    : (x) == 15 ? "In-Motion Bit. If set, the trajectory generator is presently generating a profile." \
    : "NULL")

//Latching Fault Status Register
#define R0XA4(x)\
    ((x) == 0 ? "Data flash CRC failure. This fault is considered fatal and cannot be cleared."\
    : (x) == 1 ? "Drive internal error. This fault is considered fatal and cannot be cleared."\
    : (x) == 2 ? "Short circuit."\
    : (x) == 3 ? "Drive over temperature."\
    : (x) == 4 ? "Motor over temperature."\
    : (x) == 5 ? "Over voltage."\
    : (x) == 6 ? "Under voltage."\
    : (x) == 7 ? "Feedback fault."\
    : (x) == 8 ? "Phasing error."\
    : (x) == 9 ? "Following error."\
    : (x) == 10 ? "Over Current (Latched),"\
    : (x) == 11 ? "FPGA failure. This fault is considered fatal and cannot usually be cleared. If this fault\
    occurred after a firmware download, repeating the download may clear this fault."\
    : (x) == 12 ? "Command input lost."\
    : (x) == 13 ? "Reserved."\
    : (x) == 14 ? "Safety circuit consistency check failure."\
    : (x) == 15 ? "Unable to control motor current"\
    : (x) == 16 ? "Motor wiring disconnected"\
    : (x) == 17 ? "Reserved."\
    : (x) == 18 ? "Safe torque off active."\
     : "NULL")  

//Event Status Register
#define R0XAO(x) \
    ((x) == 0 ? "Short circuit detected."\
    : (x) == 1 ? "Drive over temperature."\
    : (x) == 2 ? "Over voltage."\
    : (x) == 3 ? "Under voltage."\
    : (x) == 4 ? "Motor temperature sensor active."\
    : (x) == 5 ? "Feedback error."\
    : (x) == 6 ? "Motor phasing error."\
    : (x) == 7 ? "Current output limited."\
    : (x) == 8 ? "Voltage output limited."\
    : (x) == 9 ? "Positive limit switch active."\
    : (x) == 10 ? "Negative limit switch active."\
    : (x) == 11 ? "Enable input not active."\
    : (x) == 12 ? "Drive is disabled by software."\
    : (x) == 13 ? "Trying to stop motor."\
    : (x) == 14 ? "Motor brake activated."\
    : (x) == 15 ? "PWM outputs disabled."\
    : (x) == 16 ? "Positive software limit condition."\
    : (x) == 17 ? "Negative software limit condition."\
    : (x) == 18 ? "Tracking error."\
    : (x) == 19 ? "Tracking warning."\
    : (x) == 20 ? "Drive has been reset."\
    : (x) == 21 ? "Position has wrapped. The Position parameter cannot increase indefinitely. After reaching \
    a certain value the parameter rolls back. This type of counting is called position wrapping \
    or modulo count. Note that this bit is only active as the position wraps."\
    : (x) == 22 ? "Drive fault. A drive fault that was configured as latching has occurred. For information on \
    latching faults, see the CME 2 User Guide."\
    : (x) == 23 ? "Velocity limit has been reached."\
    : (x) == 24 ? "Acceleration limit has been reached."\
    : (x) == 25 ? "Position outside of tracking window."\
    : (x) == 26 ? "Home switch is active."\
    : (x) == 27 ? "Set if trajectory is running or motor has not yet settled into position (within Position\
    Tracking Error Limit) at the end of the move. Once the position has settled, the in motion"\
    : (x) == 28 ? "Velocity window. Set if the absolute velocity error exceeds the velocity window value."\
    : (x) == 29 ? "Phase not yet initialized. If the drive is phasing with no Halls, this bit is set until the drive \
    has initialized its phase."\
    : (x) == 30 ? "Command fault. PWM or other command signal not present."\
    : (x) == 31 ? "Not defined."\
    : "NULL" )

#endif