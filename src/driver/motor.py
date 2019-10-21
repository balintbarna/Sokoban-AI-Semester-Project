import ev3dev.ev3 as ev3
import driver.ports as prt

# this function stops control on the motors, lets them turn freely
def coast():
    left.stop_action = left.STOP_ACTION_COAST
    right.stop_action = right.STOP_ACTION_COAST
    left.stop()
    right.stop()

# this inverts the motor controller polarity (not really useful as duty can be negative)
def backwards():
    left.polarity = left.POLARITY_INVERSED
    right.polarity = right.POLARITY_INVERSED

# sets normal polarity for motor controller
def forwards():
    left.polarity = left.POLARITY_NORMAL
    right.polarity = right.POLARITY_NORMAL

# brake with the motors
def stop():
    setDuty(0)

# set voltage % for both motors, rarely used
def setDuty(duty):
    setDutyLR(duty, duty)

# set voltage % for motors separately, from -100 to 100
# in case that abs(value) is bigger than 100, both values will be divided by the same number, so that the bigger abs(value) becomes 100
def setDutyLR(dutyL, dutyR):
    # this will limit duty values between -100 and 100
    if(abs(dutyL) > 100 or abs(dutyR) > 100):
        ratio = 0.0
        if(abs(dutyL) > abs(dutyR)):
            ratio = dutyL/100.0
        else:
            ratio = dutyR/100.0
        
        # ratio should not change sign of duty values
        ratio = abs(ratio)
        dutyL = dutyL / ratio
        dutyR = dutyR / ratio
    
    # set duties according to library
    left.duty_cycle_sp = dutyL
    right.duty_cycle_sp = dutyR

def power():
    # this powers on the motors after coast()
    left.run_direct()
    right.run_direct()

#load, check connection, set default state
left = ev3.LargeMotor(prt.leftMotorPort)
right = ev3.LargeMotor(prt.rightMotorPort)
assert left.connected, "Left motor is not connected"
assert right.connected, "Right motor is not connected"

forwards()
power()
stop()